#include <Arduino.h>
#include "pins.h"
static void ch(int pin,bool on){digitalWrite(pin,on==RGB_ACTIVE_LOW?LOW:HIGH);}static void rgb(bool r,bool g,bool b){ch(PIN_RGB_R,r);ch(PIN_RGB_G,g);ch(PIN_RGB_B,b);}
void setup(){Serial.begin(115200);pinMode(PIN_RGB_R,OUTPUT);pinMode(PIN_RGB_G,OUTPUT);pinMode(PIN_RGB_B,OUTPUT);rgb(false,false,false);Serial.printf("RGB pins: R=%d G=%d B=%d\n",PIN_RGB_R,PIN_RGB_G,PIN_RGB_B);}
void loop(){Serial.println("RED");rgb(true,false,false);delay(700);Serial.println("GREEN");rgb(false,true,false);delay(700);Serial.println("BLUE");rgb(false,false,true);delay(700);Serial.println("ALL ON");rgb(true,true,true);delay(700);Serial.println("OFF");rgb(false,false,false);delay(700);}
