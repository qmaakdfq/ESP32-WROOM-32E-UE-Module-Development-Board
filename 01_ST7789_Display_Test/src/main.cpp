#include <Arduino.h>
#include <TFT_eSPI.h>
#include "pins.h"
#include "config.h"
TFT_eSPI tft;
static void backlight(bool on){digitalWrite(PIN_LCD_BL,on==LCD_BL_ACTIVE_HIGH?HIGH:LOW);}
void setup(){
  pinMode(PIN_LCD_BL,OUTPUT); backlight(false);
  tft.init(); tft.setRotation(LCD_ROTATION); tft.invertDisplay(LCD_INVERT); backlight(true);
  tft.fillScreen(TFT_BLACK); tft.drawRect(0,0,tft.width(),tft.height(),TFT_CYAN);
  tft.setTextColor(TFT_WHITE,TFT_BLACK); tft.setTextSize(2); tft.drawString("ZLX-ESP32-1",10,10);
  tft.setTextSize(1); tft.drawString("ST7789 320x240 / BGR / No Inversion",10,38);
  tft.fillRect(10,65,90,45,TFT_RED); tft.fillRect(115,65,90,45,TFT_GREEN); tft.fillRect(220,65,90,45,TFT_BLUE);
  tft.fillRect(10,125,140,45,TFT_WHITE); tft.fillRect(170,125,140,45,TFT_YELLOW);
  tft.setTextColor(TFT_GREEN,TFT_BLACK); tft.drawString("Display test PASS candidate",10,190);
  tft.setTextColor(TFT_CYAN,TFT_BLACK); tft.drawString("https://www.zlxchina.com",10,212);
}
void loop(){delay(1000);}
