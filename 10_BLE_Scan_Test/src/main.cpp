#include <Arduino.h>
#include <NimBLEDevice.h>
#include <vector>
#include <algorithm>
void setup(){Serial.begin(115200);delay(300);NimBLEDevice::init("ZLX-ESP32-1-Test");NimBLEScan* s=NimBLEDevice::getScan();s->setActiveScan(true);s->setInterval(60);s->setWindow(45);s->setMaxResults(50);NimBLEScanResults r=s->getResults(5000,false);int n=r.getCount();Serial.printf("Found %d BLE devices\n",n);for(int i=0;i<n;i++){auto* d=r.getDevice(i);if(!d)continue;String name=d->haveName()?String(d->getName().c_str()):"<Unnamed>";Serial.printf("%4d dBm %s %s\n",d->getRSSI(),d->getAddress().toString().c_str(),name.c_str());}s->clearResults();NimBLEDevice::deinit(true);}
void loop(){delay(1000);}
