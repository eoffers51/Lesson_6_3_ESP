# Session log

Claude's handoff notes between sessions. This is separate from `docs/log.md`, which is the user's own learning log (measured numbers, answered control questions).
Keep **Current state** short and up to date. Add new entries at the top of **History**, 3–6 lines each.

## Current state
- **L6.3 done (Wokwi-verified, 2026-11-09).** `main/main.c` implements all four tasks; builds with 0 warnings; reviewer OK.
- Wokwi run (public project https://wokwi.com/projects/480230992285080577, Arduino core, 1 ms tick), 55 s simulated: Sensor every 500 ms (26, 526, …); Worker at 3027/6028/9029/12031/15037 ms (vTaskDelay drift ~1 ms per cycle); free heap flat at 358396 bytes; Maintenance at 10027 and 20027 ms; after the 2 s pause, 4 catch-up Sensor lines at 12027–12029, normal rhythm from 12526.
- **Open, optional:** run on the real board (100 Hz tick) and compare. COM5 is shared with the bench camera, one at a time.
- Control questions: none in TASK.md; the learner skipped them and `docs/log.md` was not written. The "why 4?" question is now recall item R001.
- Design: priorities 2/3/4/5 (`ESP_TASK_MAIN_PRIO + 1..4`, `_Static_assert` below esp_timer); all tasks on core 1; 3072-byte stacks; Worker logs free heap.
- Reviewer notes to teach from: suspending Sensor while it holds the console lock would deadlock Maintenance's print (Wokwi: Sensor 10026, Maintenance 10027, 1 ms apart); startup create failures are only logged; stack size not measured (`uxTaskGetStackHighWaterMark`); `main/CMakeLists.txt` exports INCLUDE_DIRS ".".
- Explainer: https://claude.ai/artifact/KuKkJRba8m9Mqj2qVmfFyJ
- Toolchain: ESP-IDF v6.0.2, ESP32-S3. Output is UART only, on COM5. Native FreeRTOS API only.
- Repo `eoffers51/Lesson_6_3_ESP` is up to date (code, Wokwi files, README, logs all pushed).

## History

### 2026-11-09 — L6.3 wrap-up (task-check, close)
- Learner chose wrap-up: skipped the control questions and the "why 4?" question (now R001 in the recall bank).
- Task-check: the four "Done when" items are evidenced by the Wokwi run and by `main/main.c` (xTaskDelayUntil line 57, vTaskDelete(NULL) line 68, priorities lines 19–25). `docs/log.md` was not written by the learner.
- L6.3 marked done as simulator-verified. The real-board run is optional and open.
- Recall: R001–R004 added (box 1, next 2026-11-10). Cross-project: equipment-monitor R011 passed (box 3, next 2026-11-16); Lesson_2_3_Diods R001 passed (box 2, next 2026-11-12).

### 2026-11-09 — L6.3 brief, code and review
- Recall: R011 (equipment-monitor) answered: 10 ms tick and `pdMS_TO_TICKS(5)` = 0 correct; "doesn't wait" correct, coach added "yields only to equal priority". R001 (Lesson_2_3_Diods) answered correctly ("so the timing doesn't drift"). Grade both as passed in their own banks.
- Prediction collected: "same rhythm" after resume (see Current state).
- Learner answered why one core helps: "no switch between cores"; coach corrected to the main point (on 2 cores a higher task need not preempt, both run at once).
- Learner asked "write all for me"; firmware-engineer wrote main.c, code-reviewer OK. Coach walked through the code (priorities, `_Static_assert`, `&last_wake`, `vTaskDelete(NULL)`, function pointers, file-scope `static`).

### 2026-11-09 — L6.3 project set up
- Created `SESSION_LOG.md` and `docs/recall.md` in the project (examiner, start of task).
- Recall picks for the start of L6.3 are taken from `equipment-monitor` (R011, ticks) and `Lesson_2_3_Diods` (R001, periodic timing without drift).
