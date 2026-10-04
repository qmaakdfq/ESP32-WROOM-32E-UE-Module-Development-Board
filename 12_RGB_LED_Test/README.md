# 12 RGB_LED_Test

Strictly follows pins.h: R=GPIO22, G=GPIO17, B=GPIO16; all channels are active LOW.

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

pins.h notes that red/green behavior was observed swapped on hardware, so this example does not guess by visible color; it follows the current pins.h constants exactly.
