// L6.3 FreeRTOS task scheduling - Beetroot Academy (ESP32-S3, ESP-IDF, C)
// Source: https://github.com/eoffers51/Lesson_6_3_ESP
//
// Wokwi compiles this project with the Arduino ESP32 core, which has its own app_main().
// So main.c here renames ours to lesson_6_3_app_main() and setup() starts it.
// Everything else is the repo's ESP-IDF code.

extern "C" void lesson_6_3_app_main(void);

void setup()
{
    lesson_6_3_app_main(); // creates the four tasks and returns
}

void loop()
{
    // The Arduino loop task is not needed: delete it so it doesn't keep core 1 busy
    // (our tasks are pinned to core 1, and its Idle task must run to free deleted Workers).
    vTaskDelete(NULL);
}
