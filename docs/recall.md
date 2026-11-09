# Recall bank

Spaced-repetition items for the coach. Key points are for the coach only, never shown to the learner as answers. Boxes: 1 = 1 day, 2 = 3 days, 3 = 7 days, 4 = 14 days, 5 = 30 days.

| id | task | question | key points (for the coach only) | box | last | next |
|----|------|----------|----------------------------------|-----|------|------|
| R001 | L6.3 | Why does `xTaskDelayUntil` print a burst of 4 Sensor lines after the 2 s suspend, and not keep the same rhythm? | Misconception "same rhythm" (skipped in session). last_wake + 500 ms is already in the past, so the call returns at once and advances last_wake by one period per call: 4 missed slots in 2 s. Source: IDF v6.0.2 tasks.c:1532/1543 | 1 | 2026-11-09 | 2026-11-10 |
| R002 | L6.3 | What happens if a FreeRTOS task function returns instead of calling `vTaskDelete(NULL)` in ESP-IDF? | Port task-exit handler runs: abort with "should not return", then reboot. Forbidden by TASK.md; app_main is the exception (ESP-IDF deletes the main task). Source: IDF port.c:143-152 | 1 | 2026-11-09 | 2026-11-10 |
| R003 | L6.3 | Why can `vTaskSuspend` on another task deadlock here? | Sensor suspended while holding the console/stdout lock; Maintenance then blocks on the same lock. Priority does not free a lock held by a suspended task. Wokwi: Sensor 10026, Maintenance 10027, 1 ms apart (near miss, reviewer note 1) | 1 | 2026-11-09 | 2026-11-10 |
| R004 | L6.3 | Why does `vTaskDelay` drift while `xTaskDelayUntil` does not? Use the Worker times 3027/6028/9029 and the Sensor times 26/526/1026. | vTaskDelay counts from the call, so each cycle adds the time spent running and tick rounding (Worker ~1 ms per cycle). xTaskDelayUntil adds the period to the previous wake time, so the grid stays fixed (Sensor exact 500 ms) | 1 | 2026-11-09 | 2026-11-10 |
