# 07 TF_Card_Format_DESTRUCTIVE

Standalone destructive format example. Disabled by default; ALLOW_DESTRUCTIVE_FORMAT must be set to true. It erases files on the card.

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

Warning: this example erases TF-card data. Mark it as Advanced / Data will be erased on your website.
