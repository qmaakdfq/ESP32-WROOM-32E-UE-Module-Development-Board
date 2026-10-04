# 08 Audio_PWM_and_Amplifier_Test

Tests GPIO26 AUDIO_IN PWM and GPIO4 AUDIO_EN with a 1 kHz tone and 200 Hz-4 kHz sweep, then fully stops PWM.

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
