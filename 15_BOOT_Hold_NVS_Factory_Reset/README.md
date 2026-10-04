# 15 BOOT_Hold_NVS_Factory_Reset

Demonstrates the factory firmware BOOT-hold reset logic: hold for 5 seconds to erase ESP32 NVS and restart.

## Project files

- `src/main.cpp`: standalone example
- `include/pins.h`: actual board GPIO mapping
- `include/config.h`: actual display/touch/ADC settings
- `platformio.ini`: PlatformIO configuration

## Usage

1. Open this folder with VS Code + PlatformIO.
2. Build and flash the ZLX-ESP32-1.
3. Serial examples use 115200 baud.

Downloads & Resources: https://www.zlxchina.com  
Technical Support: mq19880204@gmail.com

## Special note

This erases Preferences/Wi-Fi and other data stored in ESP32 NVS. It does not format the TF card.
