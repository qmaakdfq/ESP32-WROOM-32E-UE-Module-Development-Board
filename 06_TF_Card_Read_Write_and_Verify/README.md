# 06 TF_Card_Read_Write_and_Verify

Creates /factory_test.txt, performs timed write/read/content verification, and does not format the card.

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
