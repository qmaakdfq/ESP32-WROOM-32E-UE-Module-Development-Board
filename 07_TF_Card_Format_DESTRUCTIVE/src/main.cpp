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

static constexpr bool ALLOW_DESTRUCTIVE_FORMAT=false;
void setup(){Serial.begin(115200);delay(300);if(!ALLOW_DESTRUCTIVE_FORMAT){Serial.println("FORMAT DISABLED. Set ALLOW_DESTRUCTIVE_FORMAT=true only after backing up the card.");return;}sdBusBegin();uint64_t cap=0;if(sdMount(false)){cap=SD.cardSize();uint8_t zero[512]={0};bool erased=true;for(uint32_t sec=0;sec<8&&erased;sec++)erased=SD.writeRAW(zero,sec);Serial.printf("Boot area erase: %s\n",erased?"OK":"FAIL");sdRelease();delay(150);}bool ok=sdMount(true);if(!ok){Serial.println("FORMAT FAIL");return;}if(!cap)cap=SD.cardSize();File f=SD.open("/factory_test.txt",FILE_WRITE);bool verify=false;if(f){f.print("ZLX-ESP32-1 format verify");f.close();f=SD.open("/factory_test.txt",FILE_READ);if(f){String s=f.readString();verify=s.indexOf("ZLX-ESP32-1")>=0;f.close();}}Serial.printf("FORMAT DONE: %lluMB verify=%s\n",(unsigned long long)(cap/1048576ULL),verify?"PASS":"FAIL");sdRelease();}
void loop(){delay(1000);}
