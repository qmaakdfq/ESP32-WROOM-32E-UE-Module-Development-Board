#include <Arduino.h>
#include <TFT_eSPI.h>
#include "pins.h"
#include "config.h"
TFT_eSPI tft;

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

void setup(){pinMode(PIN_LCD_BL,OUTPUT);digitalWrite(PIN_LCD_BL,LCD_BL_ACTIVE_HIGH?HIGH:LOW);touchBegin();tft.init();tft.setRotation(LCD_ROTATION);tft.invertDisplay(LCD_INVERT);tft.fillScreen(TFT_BLACK);tft.setTextColor(TFT_WHITE,TFT_BLACK);tft.drawString("XPT2046 mapped touch test",8,8);}
void loop(){if(digitalRead(PIN_RTP_IRQ)==LOW){TouchPoint p=touchMap(touchReadRaw());if(p.valid){tft.fillCircle(p.x,p.y,2,TFT_GREEN);Serial.printf("XY=%d,%d\n",p.x,p.y);}delay(15);}delay(2);}
