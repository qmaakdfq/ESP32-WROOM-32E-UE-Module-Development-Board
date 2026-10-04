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

void setup(){Serial.begin(115200);delay(300);sdBusBegin();if(!sdMount(false)){Serial.println("TF mount FAIL");return;}uint8_t t=SD.cardType();const char* n=t==CARD_MMC?"MMC":t==CARD_SD?"SDSC":t==CARD_SDHC?"SDHC/SDXC":"Unknown";Serial.printf("Type=%s Card=%lluMB Total=%lluMB Used=%lluMB\n",n,(unsigned long long)(SD.cardSize()/1048576ULL),(unsigned long long)(SD.totalBytes()/1048576ULL),(unsigned long long)(SD.usedBytes()/1048576ULL));File root=SD.open("/");for(int i=0;root&&i<8;i++){File f=root.openNextFile();if(!f)break;Serial.printf("%s %s\n",f.isDirectory()?"[DIR]":"[FILE]",f.name());f.close();}if(root)root.close();sdRelease();}
void loop(){delay(1000);}
