# 11 GT-U7_GPS_Receive_Test

Releases UART0 GPIO1, remaps UART2 RX to GPIO1, reads GT-U7 NMEA at 9600, and shows fix information on the LCD.

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

GPIO1 is shared with CH340/UART0 TX. Disconnect GPS while flashing. USB serial output stops after GPS takes GPIO1; GPS information is shown on the LCD.
