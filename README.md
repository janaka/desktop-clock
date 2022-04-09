# Desktop Clock

A desktop digital clock built using an ESP32 microcontroller, MAX7219 led matrix display, and Arduino lang.

- Syncs time from an Internet time server (NTP) on boot-up
- Syncs time once a day from Internet servers to prevent drift.
- Connects to WiFi just before Internet time sync and then disconnects when done to conserve power
- Automatically adjusts UK daylight saving

## Design Principles

- Optimise for power consumption.
  - Minimise external service dependencies.
  - JIT connect to WiFi for external calls. Disconnect immediately after.
- Minimise third part library dependencies.

## Dependencies

- SPI.h
- WiFi.h
- LedMatrix.h [MAX7219 library](https://github.com/janaka/esp32-led-matrix). Forked from [@nhatuan84](https://github.com/nhatuan84/esp32-led-matrix) and modified to support the max7219 hardware variation I have.
- time.h [ref](https://en.wikibooks.org/wiki/C_Programming/time.h), [source](https://github.com/esp8266/Arduino/blob/master/cores/esp8266/time.cpp) - standard C library.
  - Not using TimeLib.h by Michael Margolis because we don't need all that functionality. [Github](https://www.pjrc.com/teensy/td_libs_Time.html), [Docs](https://www.pjrc.com/teensy/td_libs_Time.html)

## Contributions

Note: the following has only been tested on macOS.

### Third-party libraries

- Open Arduino IDE > preferences, note the sketchbook location
- `cd <sketchbook location>/libraries`
- `git clone https://github.com/janaka/esp32-led-matrix`

### Tool Dependencies

Just the Arduino IDE
[Arduino IDE](https://www.arduino.cc/en/main/software#download)

Board manager URLs - one off config

- `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
- `https://dl.espressif.com/dl/package_esp32_index.json`

Using VS Code

[Arduino VS Code extension](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino)
Depends on the Arduino IDE

### Configure WiFi

- Rename `1.wifi_config.tmp` to `1.wifi_config.ino`
  - NOTE: the `1.wifi_config.ino` is in `.gitignore`. **DO NOT** check-in
- Add your wifi details.

### Uploading a sketch in VS Code

- Serial port should show as `/dev/tty.SLAB_USBtoUART`.
  - this needs the SLAB driver to be install
- Upload from VS Code: `option + command + u`.

### Debug

Look at the serial monitor output for information using the Arduino IDE

### Hardware wiring diagram

TODO: diagram

- CS  -> GPIO 15
- CLK -> GPIO 14
- DIN -> GPIO 12

## ESP-WROM-32 devkit pinout schema

You may have a different variation of the ESP32 dev board. Check before connecting.

![ESP-WROM-32 devkit pinout schema](./docs/assets/esp-wrom-32_devkit_schema.jpg)