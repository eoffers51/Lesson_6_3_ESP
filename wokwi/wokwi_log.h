// Wokwi-only: the Arduino ESP32 core hides ESP_LOGx output, but this exercise is
// checked by its log lines. Send them to printf in the same "I (ms) TAG: text" format.
// Included before main.c's own code (see wokwi/README.md). Not used by the real firmware.
#pragma once
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#undef ESP_LOGI
#undef ESP_LOGE
#define WOKWI_LOG(letter, tag, fmt, ...)                                                                              \
    printf(letter " (%lu) %s: " fmt "\n", (unsigned long)pdTICKS_TO_MS(xTaskGetTickCount()), tag, ##__VA_ARGS__)
#define ESP_LOGI(tag, fmt, ...) WOKWI_LOG("I", tag, fmt, ##__VA_ARGS__)
#define ESP_LOGE(tag, fmt, ...) WOKWI_LOG("E", tag, fmt, ##__VA_ARGS__)
