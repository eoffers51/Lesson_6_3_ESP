# Session log

Claude's handoff notes between sessions. This is separate from `docs/log.md`, which is the user's own learning log (measured numbers, answered control questions).
Keep **Current state** short and up to date. Add new entries at the top of **History**, 3–6 lines each.

## Current state
- **L6.3 code written and reviewed, not run yet** (2026-11-09). `main/main.c` implements all four tasks (firmware-engineer, on the learner's "write all for me"); builds with 0 warnings. Reviewer verdict: OK, nothing must change.
- **Next step: verify in the Wokwi online simulator** (learner's choice; the board was not flashed). Use the `wiring-docs` skill for `diagram.json` + `wokwi.toml` (board only, no external parts) and ask before saving the public wokwi.com project. Then check the log against "Done when" in TASK.md, then control questions, then wrap-up.
- **Open prediction:** learner predicted the `[Sensor]` lines come back at the "same rhythm" after the 2 s suspend. Expected: a burst of 4–5 catch-up lines at ~12.0 s (xTaskDelayUntil, tasks.c:1532/1543), then the normal 500 ms rhythm. Do not reveal before the log is seen. The explainer's pause section is still closed for the learner.
- Design: priorities 2/3/4/5 (`ESP_TASK_MAIN_PRIO + 1..4`, `_Static_assert` below esp_timer); all tasks pinned to core 1; 3072-byte stacks; Worker logs free heap (leak check).
- Reviewer notes to teach from: (1) suspending Sensor while it holds the console lock would deadlock Maintenance's print (not reachable with current tick phase); (2) startup create failures are only logged, and main.c:111 uses TAG_SENSOR from app_main; (3) Worker line has a free-heap suffix after the required text; (4) stack size not measured (`uxTaskGetStackHighWaterMark`); (5) `main/CMakeLists.txt` exports INCLUDE_DIRS ".".
- Explainer: https://claude.ai/artifact/KuKkJRba8m9Mqj2qVmfFyJ
- Toolchain: ESP-IDF v6.0.2, ESP32-S3, tick 100 Hz. Output is UART only, on COM5 (shared with the bench camera serial capture, one at a time). Native FreeRTOS API only.
- Not committed since the setup commits: `main/main.c`, `SESSION_LOG.md`, `docs/recall.md`.

## History

### 2026-11-09 — L6.3 brief, code and review
- Recall: R011 (equipment-monitor) answered: 10 ms tick and `pdMS_TO_TICKS(5)` = 0 correct; "doesn't wait" correct, coach added "yields only to equal priority". R001 (Lesson_2_3_Diods) answered correctly ("so the timing doesn't drift"). Grade both as passed in their own banks.
- Prediction collected: "same rhythm" after resume (see Current state).
- Learner answered why one core helps: "no switch between cores"; coach corrected to the main point (on 2 cores a higher task need not preempt, both run at once).
- Learner asked "write all for me"; firmware-engineer wrote main.c, code-reviewer OK. Coach walked through the code (priorities, `_Static_assert`, `&last_wake`, `vTaskDelete(NULL)`, function pointers, file-scope `static`).

### 2026-11-09 — L6.3 project set up
- Created `SESSION_LOG.md` and `docs/recall.md` in the project (examiner, start of task).
- Recall picks for the start of L6.3 are taken from `equipment-monitor` (R011, ticks) and `Lesson_2_3_Diods` (R001, periodic timing without drift).
