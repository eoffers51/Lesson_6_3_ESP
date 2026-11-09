# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

ESP-IDF **v6.0.2** project for an **ESP32-S3-DevKitC-1** clone (Beetroot Academy homework "FreeRTOS Task Scheduling"). The requirements are in `TASK.md`: four tasks (Sensor Reader, Event Trigger, Worker, Maintenance) with fixed periods and priorities. Check changes against it.

## Build / flash

`idf.py` is not on PATH. Activate the IDF env in the same PowerShell call as every `idf.py` command (shell state doesn't persist between calls):

```powershell
. C:\Espressif\tools\Microsoft.v6.0.2.PowerShell_profile.ps1 *> $null; idf.py build
```

- Activating the env also puts Espressif's `cmake`/`ninja` ahead of the STM32CubeCLT ones on PATH, so never build without it.
- `sdkconfig` is gitignored and may not exist. If it's missing, run `idf.py set-target esp32s3` before the first build.
- Flash with `idf.py -p COM5 flash` (or use `/build-flash`). Never run `idf.py monitor`: it's interactive and will hang, so ask the user to run it or capture a timed log via `build-ops`.
- clangd reads `compile_commands.json` from a hardcoded `build/` path in `.vscode/settings.json`, so IDE diagnostics are only valid after a first build.
- The `esp-idf-source` skill and `idf-librarian` look up v6.1. This project builds against v6.0.2, so double-check anything they report as new in 6.1.

## Task rules (graded)

`TASK.md` is written in CMSIS-OS terms. Use the **native FreeRTOS API only** (`xTaskCreate`, `vTaskDelay`, `vTaskSuspend`, `vTaskResume`, `vTaskDelete`, ...), never `osDelay` / `osThread*`.
- The priority names Low / Normal / High / Realtime have to be mapped to numeric FreeRTOS priorities, ordered Low < Normal < High < Realtime and all below `configMAX_PRIORITIES`.
- Sensor Reader must not use plain `vTaskDelay` for its 500 ms period.
- Worker must end with `vTaskDelete(NULL)`. Returning from the task function or falling off its closing `}` is forbidden.
- Results are checked in the serial log on COM5 (the exact `[Sensor]`, `[Worker]`, `[Maintenance]` strings from TASK.md).

## Hardware

UART output only: just the board on COM5 (CH343 USB-UART). There are no external LEDs or buttons. The small green LED that flickers with log output is the UART TX light.

## Repo

The project goes in GitHub repo `eoffers51/Lesson_6_3_ESP` (not created yet; follow the academy rules in the parent CLAUDE.md).

## Code style

Formatting follows `.clang-format` (ESP-IDF style: 4 spaces, function braces on their own line). A hook applies it after edits.
