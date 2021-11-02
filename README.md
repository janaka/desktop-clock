# Desktop Clock

A desktop digital clock built using an ESP32 microcontroller, MAX7219 led matrix display, and Arduino lang.

It syncs time from an Internet time server on boot-up and then once a day while powered.

Uses [this MAX7219 library](https://github.com/janaka/esp32-led-matrix). Forked from [@nhatuan84](https://github.com/nhatuan84/esp32-led-matrix) and modified to support the max7219 hardware variation I have.

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

Look at the serial monitor output for information.

### Hardware wiring diagram

TODO: diagram

- CS  -> GPIO 15
- CLK -> GPIO 14
- DIN -> GPIO 12

## ESP-WROM-32 devkit pinout schema

You may have a different variation of the ESP32 dev board. Check before connecting.

![ESP-WROM-32 devkit pinout schema](./docs/assets/esp-wrom-32_devkit_schema.jpg)