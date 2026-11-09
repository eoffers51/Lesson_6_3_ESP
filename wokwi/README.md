# Wokwi-only files

The [Wokwi project](https://wokwi.com/projects/480230992285080577) holds a copy of this repo's code so it runs in the browser. Wokwi compiles ESP-IDF code with the **Arduino ESP32 core**, which needs small changes:

| File in Wokwi | Comes from |
|---|---|
| `sketch.ino` | [`wokwi/sketch.ino`](sketch.ino): `setup()` calls our `app_main`, renamed to `lesson_6_3_app_main` (the Arduino core already has an `app_main`). `loop()` deletes the Arduino loop task. |
| `wokwi_log.h` | [`wokwi/wokwi_log.h`](wokwi_log.h): routes `ESP_LOGI`/`ESP_LOGE` to `printf` (the core hides ESP_LOG output) |
| `main.c` | `main/main.c` with these lines added at the top: `#define app_main lesson_6_3_app_main` and `#include "wokwi_log.h"` |
| `diagram.json` | [`diagram.json`](../diagram.json) |

When the code changes, regenerate `main.c` (Git Bash, from the repo root):

```sh
mkdir -p build/wokwi
{ printf '#define app_main lesson_6_3_app_main\n#include "wokwi_log.h"\n\n'; cat main/main.c; } > build/wokwi/main.c
```

Upload it to the Wokwi project again (file menu ▾ → *Upload file(s)…*) and press **Save**.
