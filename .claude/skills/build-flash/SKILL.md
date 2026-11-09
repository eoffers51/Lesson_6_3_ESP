---
name: build-flash
description: Build this ESP-IDF project and flash it to the ESP32-S3 board on COM5. Use when the user asks to build, flash, upload, or "run it on the board". Optional argument is the serial port (default COM5).
---

Build and flash the firmware. Port argument: `$ARGUMENTS` (empty → `COM5`).

Run every step with the PowerShell tool, prefixing each command with the env activation in the same call:

```powershell
. C:\Espressif\tools\Microsoft.v6.0.2.PowerShell_profile.ps1 *> $null; <command>
```

1. If `sdkconfig` does not exist at the project root, run `idf.py set-target esp32s3`.
2. Run `idf.py build` (timeout 600000, because first builds are slow). If it fails, show the first compiler error with `file:line`, then stop. This is a learning project, so explain the cause and leave the fix to the user unless they asked for it. Stop here as well if the user only asked to build.
3. Port: use `$ARGUMENTS` if given, otherwise `COM5`. COM5 is shared with the bench camera's serial capture. If it is busy (flash reports it can't open the port), tell the user to close the monitor or capture and do not kill anything.
4. Run `idf.py -p <PORT> flash`. If it can't connect, tell the user to hold BOOT, tap RESET, release BOOT, then retry once.
5. Report the result. Do NOT run `idf.py monitor` (interactive, will hang). Tell the user to run `idf.py -p <PORT> monitor` themselves (Ctrl+] to exit), or delegate a timed log capture to `build-ops`.
