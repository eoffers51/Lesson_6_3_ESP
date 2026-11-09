# Lesson 6.3 — FreeRTOS task scheduling (ESP32-S3)

[![Simulate on Wokwi](https://img.shields.io/badge/simulate-Wokwi-6c4cbb)](https://wokwi.com/projects/480230992285080577)

Beetroot Academy exercise: four FreeRTOS tasks with different priorities share one CPU core. Requirements: [`TASK.md`](TASK.md).

| Task | Priority | What it does |
|---|---|---|
| Event Trigger | Low (2) | Sleeps 3 s with `vTaskDelay`, then creates a Worker |
| Sensor Reader | Normal (3) | Prints `[Sensor] Data read...` every 500 ms on a fixed grid (`xTaskDelayUntil`) |
| Worker | High (4) | Prints `[Worker] Processing event...` and the free heap, then `vTaskDelete(NULL)` |
| Maintenance | Realtime (5) | Every 10 s suspends the Sensor for 2 s, then resumes it |

All four tasks are pinned to core 1, so priority alone decides which one runs. Built with ESP-IDF v6.0.2 for the ESP32-S3-DevKitC-1.

## Hardware

Only the board. Output goes to the serial console (UART, 115200 baud). There are no external parts, so there is no wiring diagram.

## Build and run

```sh
idf.py set-target esp32s3   # first time only
idf.py build
idf.py -p <PORT> flash monitor
```

## Simulate in Wokwi

**[▶ Open the Wokwi project](https://wokwi.com/projects/480230992285080577)** and press the green ▶ button. The first build can take a minute or two on Wokwi's free servers, and the simulation may run slower than real time.

The Wokwi project runs this repo's own `main/main.c`. Wokwi compiles ESP-IDF code with the Arduino ESP32 core, so [`wokwi/`](wokwi) has two small Wokwi-only files:

- `sketch.ino` starts our `app_main`. It is renamed because the core already has one.
- `wokwi_log.h` sends `ESP_LOGI`/`ESP_LOGE` to `printf`, because the Arduino core hides those logs.

In the simulator the FreeRTOS tick is 1 ms (Arduino core) instead of 10 ms, so timestamps step in 1 ms.

What to look for in the serial monitor:
- `[Sensor]` lines every 500 ms.
- A `[Worker]` line every ~3 s. The free heap stays the same, so deleted Workers are freed.
- `[Maintenance]` at 10 s, 20 s, … followed by 2 s without `[Sensor]` lines.
- Right after each pause, a short burst of `[Sensor]` lines: `xTaskDelayUntil` catches up the missed 500 ms slots.

**VS Code:** install the *Wokwi Simulator* extension, run `idf.py build`, then *Wokwi: Start Simulator*. This runs the real firmware: [`wokwi.toml`](wokwi.toml) points to the build, and [`diagram.json`](diagram.json) holds the circuit.
