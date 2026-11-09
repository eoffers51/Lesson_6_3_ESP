/*
 * L6.3 FreeRTOS task scheduling: four tasks with different priorities.
 *
 *   Sensor Reader  (Normal)   prints every 500 ms, strictly periodic
 *   Event Trigger  (Low)      every 3 s creates a short-lived Worker
 *   Worker         (High)     prints once, then deletes itself
 *   Maintenance    (Realtime) every 10 s pauses the Sensor for 2 s
 */
#include <inttypes.h> // PRIu32: portable printf format for uint32_t
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h" // esp_get_free_heap_size()
#include "esp_task.h"   // ESP_TASK_MAIN_PRIO, ESP_TASK_TIMER_PRIO

/* Priorities: a bigger number wins. app_main runs at ESP_TASK_MAIN_PRIO (1), so we start
 * just above it. ESP-IDF's own tasks sit near the top (esp_timer 22, ipc 24), so 2..5
 * stays far away from them and our tasks never block the system. */
#define PRIO_LOW (ESP_TASK_MAIN_PRIO + 1)      // 2: Event Trigger
#define PRIO_NORMAL (ESP_TASK_MAIN_PRIO + 2)   // 3: Sensor Reader
#define PRIO_HIGH (ESP_TASK_MAIN_PRIO + 3)     // 4: Worker
#define PRIO_REALTIME (ESP_TASK_MAIN_PRIO + 4) // 5: Maintenance

// Fails the build if someone raises our priorities into the range of the system tasks.
_Static_assert(PRIO_REALTIME < ESP_TASK_TIMER_PRIO, "app priorities must stay below esp_timer");

/* ESP32-S3 has two cores and FreeRTOS runs tasks on both at once. Pinning all our tasks to
 * one core makes them share a single CPU, so priority alone decides who runs (as in the lesson). */
#define APP_CORE 1

/* Stack size in bytes (ESP-IDF counts bytes, vanilla FreeRTOS counts words).
 * ESP_LOGx formats its text on the calling task's stack; 3 KB is a common safe size for tasks that log. */
#define TASK_STACK_BYTES 3072

#define SENSOR_PERIOD_MS 500
#define TRIGGER_SLEEP_MS 3000
#define MAINTENANCE_PERIOD_MS 10000
#define MAINTENANCE_PAUSE_MS 2000

// One TAG per task, so every log line shows which task wrote it.
static const char *TAG_SENSOR = "Sensor";
static const char *TAG_TRIGGER = "Trigger";
static const char *TAG_WORKER = "Worker";
static const char *TAG_MAINT = "Maintenance";

// Written once in app_main, read by Maintenance to suspend/resume the Sensor.
static TaskHandle_t s_sensor_handle = NULL;

static void sensor_task(void *arg)
{
    // Take the start time once: every wake-up is then "previous wake + 500 ms",
    // so the time spent printing does not add up into drift.
    TickType_t last_wake = xTaskGetTickCount();

    while (1) {
        ESP_LOGI(TAG_SENSOR, "[Sensor] Data read...");
        xTaskDelayUntil(&last_wake, pdMS_TO_TICKS(SENSOR_PERIOD_MS));
    }
}

static void worker_task(void *arg)
{
    // Free heap is logged so that a leak (a Worker never freed) would show as a falling number.
    ESP_LOGI(TAG_WORKER, "[Worker] Processing event... (free heap: %" PRIu32 " bytes)", esp_get_free_heap_size());

    // A FreeRTOS task function must never return. NULL means "delete myself";
    // the Idle task frees this task's stack and TCB afterwards.
    vTaskDelete(NULL);
}

static void event_trigger_task(void *arg)
{
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(TRIGGER_SLEEP_MS));

        // The Worker deletes itself, so we don't need its handle (NULL).
        BaseType_t ok =
            xTaskCreatePinnedToCore(worker_task, "worker", TASK_STACK_BYTES, NULL, PRIO_HIGH, NULL, APP_CORE);
        if (ok != pdPASS) {
            // Usually means the heap is full; try again on the next event instead of crashing.
            ESP_LOGE(TAG_TRIGGER, "Failed to create Worker task");
        }
    }
}

static void maintenance_task(void *arg)
{
    TickType_t last_wake = xTaskGetTickCount();

    while (1) {
        xTaskDelayUntil(&last_wake, pdMS_TO_TICKS(MAINTENANCE_PERIOD_MS));

        // vTaskSuspend(NULL) would suspend THIS task, so never pass a missing handle.
        if (s_sensor_handle == NULL) {
            ESP_LOGE(TAG_MAINT, "Sensor task handle missing, skipping maintenance");
            continue;
        }

        vTaskSuspend(s_sensor_handle);
        ESP_LOGI(TAG_MAINT, "[Maintenance] System paused for 2 seconds.");
        vTaskDelay(pdMS_TO_TICKS(MAINTENANCE_PAUSE_MS));
        vTaskResume(s_sensor_handle);
    }
}

void app_main(void)
{
    // Sensor first: Maintenance needs its handle.
    if (xTaskCreatePinnedToCore(sensor_task, "sensor", TASK_STACK_BYTES, NULL, PRIO_NORMAL, &s_sensor_handle,
                                APP_CORE) != pdPASS) {
        ESP_LOGE(TAG_SENSOR, "Failed to create Sensor task");
    }

    if (xTaskCreatePinnedToCore(event_trigger_task, "trigger", TASK_STACK_BYTES, NULL, PRIO_LOW, NULL, APP_CORE) !=
        pdPASS) {
        ESP_LOGE(TAG_TRIGGER, "Failed to create Event Trigger task");
    }

    if (xTaskCreatePinnedToCore(maintenance_task, "maint", TASK_STACK_BYTES, NULL, PRIO_REALTIME, NULL, APP_CORE) !=
        pdPASS) {
        ESP_LOGE(TAG_MAINT, "Failed to create Maintenance task");
    }

    // Returning is allowed here only because app_main is special: ESP-IDF deletes the
    // main task for us afterwards. Our own tasks (like the Worker) must call vTaskDelete.
}
