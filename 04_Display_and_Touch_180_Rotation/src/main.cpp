#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Preferences.h>
#include "pins.h"
#include "config.h"
TFT_eSPI tft; Preferences prefs; bool rot180=false;

struct TouchRaw { int16_t x=0,y=0,z=0; };
struct TouchPoint { int16_t x=0,y=0; bool valid=false; };

static uint8_t touchTransfer(uint8_t value) {
  uint8_t result=0;
  const uint32_t d=max<uint32_t>(1,500000UL/TOUCH_SPI_HZ);
  for(int bit=7;bit>=0;--bit){
    digitalWrite(PIN_RTP_DIN,(value>>bit)&1);
    delayMicroseconds(d); digitalWrite(PIN_RTP_SCK,HIGH);
    result=(result<<1)|digitalRead(PIN_RTP_DOUT);
    delayMicroseconds(d); digitalWrite(PIN_RTP_SCK,LOW);
  }
  return result;
}
static uint16_t touchRead12(uint8_t cmd){
  touchTransfer(cmd);
  uint16_t v=(uint16_t(touchTransfer(0))<<8)|touchTransfer(0);
  return (v>>3)&0x0FFF;
}
static void touchBegin(){
  pinMode(PIN_RTP_SCK,OUTPUT); pinMode(PIN_RTP_DIN,OUTPUT);
  pinMode(PIN_RTP_DOUT,INPUT); pinMode(PIN_RTP_CS,OUTPUT);
  pinMode(PIN_RTP_IRQ,INPUT_PULLUP);
  digitalWrite(PIN_RTP_CS,HIGH); digitalWrite(PIN_RTP_SCK,LOW);
}
static TouchRaw touchReadRaw(){
  TouchRaw r; if(digitalRead(PIN_RTP_IRQ)!=LOW) return r;
  long sx=0,sy=0,sz=0;
  for(int i=0;i<5;i++){
    digitalWrite(PIN_RTP_CS,LOW);
    uint16_t x=touchRead12(0xD0), y=touchRead12(0x90), z1=touchRead12(0xB0), z2=touchRead12(0xC0);
    digitalWrite(PIN_RTP_CS,HIGH);
    sx+=x; sy+=y; sz+=z1+4095-z2;
  }
  r.x=sx/5; r.y=sy/5; r.z=max<long>(0,sz/5); return r;
}
static TouchPoint touchMap(const TouchRaw& raw,bool rotate180=false){
  TouchPoint p; if(raw.z<TOUCH_MIN_PRESSURE) return p;
  const int32_t ax=TOUCH_SWAP_XY?raw.y:raw.x;
  const int32_t ay=TOUCH_SWAP_XY?raw.x:raw.y;
  long x=map(ax,TOUCH_RAW_X_MIN,TOUCH_RAW_X_MAX,0,LCD_WIDTH-1);
  long y=map(ay,TOUCH_RAW_Y_MIN,TOUCH_RAW_Y_MAX,0,LCD_HEIGHT-1);
  x=constrain(x,0L,long(LCD_WIDTH-1)); y=constrain(y,0L,long(LCD_HEIGHT-1));
  if(TOUCH_INVERT_X) x=LCD_WIDTH-1-x;
  if(TOUCH_INVERT_Y) y=LCD_HEIGHT-1-y;
  if(rotate180){ x=LCD_WIDTH-1-x; y=LCD_HEIGHT-1-y; }
  p.x=x; p.y=y; p.valid=true; return p;
}

static void drawUI(){tft.fillScreen(TFT_BLACK);tft.setTextColor(TFT_WHITE,TFT_BLACK);tft.setTextSize(2);tft.drawString("ZLX-ESP32-1",8,8);tft.setTextSize(1);tft.drawString(rot180?"Display + Touch: 180 deg":"Display + Touch: 0 deg",8,36);tft.fillRoundRect(244,4,72,30,5,TFT_ORANGE);tft.setTextColor(TFT_BLACK,TFT_ORANGE);tft.drawCentreString("ROTATE",280,14,1);tft.setTextColor(TFT_CYAN,TFT_BLACK);tft.drawString("Tap screen to draw",8,220);}
static void applyRotation(){uint8_t r=rot180?((LCD_ROTATION+2)&3):LCD_ROTATION;tft.setRotation(r);prefs.putBool("rot180",rot180);drawUI();}
void setup(){pinMode(PIN_LCD_BL,OUTPUT);digitalWrite(PIN_LCD_BL,LCD_BL_ACTIVE_HIGH?HIGH:LOW);touchBegin();prefs.begin("zlx-demo",false);rot180=prefs.getBool("rot180",false);tft.init();tft.invertDisplay(LCD_INVERT);tft.setRotation(rot180?((LCD_ROTATION+2)&3):LCD_ROTATION);drawUI();}
void loop(){if(digitalRead(PIN_RTP_IRQ)==LOW){TouchPoint p=touchMap(touchReadRaw(),rot180);if(p.valid){if(p.x>=244&&p.y<40){rot180=!rot180;applyRotation();delay(350);}else tft.fillCircle(p.x,p.y,2,TFT_GREEN);}delay(12);}delay(2);}
