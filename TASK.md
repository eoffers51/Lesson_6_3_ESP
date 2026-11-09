# FreeRTOS Task Scheduling Exercise

Build a system of four tasks with a clear division of responsibilities and priorities.

---

## 1. Task "Sensor Reader" (priority: Normal)

- Simulates polling a sensor. Prints to UART: `[Sensor] Data read...`
- Strict periodicity: exactly every **500 ms**. Using a plain `vTaskDelay` is **not allowed**.

## 2. Task "Event Trigger" (priority: Low)

- Simulates an unpredictable external event.
- Sleeps for **3 s** using `vTaskDelay` / `osDelay`.
- On waking up, dynamically creates a new "Worker Task", then goes back to sleep.

## 3. Task "Worker Task" (priority: High)

- Created only when an event occurs (from Task 2). Prints `[Worker] Processing event...`
- Must then terminate itself with `vTaskDelete(NULL)` (or `osThreadExit()` / `osThreadTerminate()`).
- Exiting via `return` or by reaching the closing `}` without deleting the task is **forbidden**.

## 4. Task "Maintenance Mode" (priority: Realtime)

- Wakes up once every **10 s**.
- Pauses "Sensor Reader" with `vTaskSuspend(handle)` / `osThreadSuspend`.
- Prints `[Maintenance] System paused for 2 seconds.` and waits **2 s**.
- Resumes the sensor with `vTaskResume` / `osThreadResume`.
