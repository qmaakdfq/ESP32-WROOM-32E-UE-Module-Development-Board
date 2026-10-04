#include <Arduino.h>
#include "pins.h"
static bool pressed(){return digitalRead(PIN_BOOT_AUTO)==(BOOT_ACTIVE_LOW?LOW:HIGH);}void setup(){Serial.begin(115200);pinMode(PIN_BOOT_AUTO,INPUT_PULLUP);Serial.println("Press BOOT...");}
void loop(){static bool old=false;bool now=pressed();if(now!=old){old=now;Serial.println(now?"BOOT: PRESSED":"BOOT: RELEASED");}delay(10);}
