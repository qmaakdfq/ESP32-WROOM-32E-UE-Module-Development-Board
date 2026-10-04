#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "pins.h"
#include "config.h"
static bool sdReady=false;
static void sdBusBegin(){pinMode(PIN_SD_CS,OUTPUT);digitalWrite(PIN_SD_CS,HIGH);SPI.begin(PIN_SD_SCK,PIN_SD_MISO,PIN_SD_MOSI,PIN_SD_CS);}
static void sdRelease(){if(sdReady){SD.end();sdReady=false;}digitalWrite(PIN_SD_CS,HIGH);}
static bool sdMount(bool formatIfEmpty=false){
  sdRelease(); const uint32_t speed[3]={SD_SPI_HZ,2000000UL,1000000UL};
  for(int i=0;i<3;i++){digitalWrite(PIN_SD_CS,HIGH);delay(i?100:30);Serial.printf("Mount try %d @ %lu Hz\n",i+1,(unsigned long)speed[i]);sdReady=SD.begin(PIN_SD_CS,SPI,speed[i],"/sd",5,formatIfEmpty);if(sdReady&&SD.cardType()!=CARD_NONE)return true;if(sdReady){SD.end();sdReady=false;}}
  return false;
}

void setup(){Serial.begin(115200);delay(300);sdBusBegin();if(!sdMount(false)){Serial.println("Mount FAIL");return;}const char* path="/factory_test.txt";String token="ZLX-ESP32-1 factory test\r\ntime="+String(millis())+"\r\n";SD.remove(path);uint32_t t0=millis();File f=SD.open(path,FILE_WRITE);size_t w=0;if(f){w=f.write((const uint8_t*)token.c_str(),token.length());f.flush();f.close();}uint32_t wt=millis()-t0;String got;got.reserve(SD_MAX_TEST_BYTES);t0=millis();f=SD.open(path,FILE_READ);uint32_t deadline=millis()+SD_OPERATION_TIMEOUT_MS;bool timeout=false;while(f&&got.length()<SD_MAX_TEST_BYTES){if((int32_t)(deadline-millis())<=0){timeout=true;break;}int c=f.read();if(c<0)break;got+=char(c);}if(f)f.close();uint32_t rt=millis()-t0;bool pass=(w==token.length()&&!timeout&&got==token);Serial.printf("Write=%uB/%lums Read=%uB/%lums Compare=%s\n",(unsigned)w,(unsigned long)wt,(unsigned)got.length(),(unsigned long)rt,pass?"PASS":"FAIL");sdRelease();}
void loop(){delay(1000);}
