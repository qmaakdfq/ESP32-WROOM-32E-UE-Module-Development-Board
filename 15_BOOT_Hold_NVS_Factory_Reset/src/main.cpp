#include <Arduino.h>
#include <nvs_flash.h>
#include "pins.h"
#include "config.h"
static bool pressed(){return digitalRead(PIN_BOOT_AUTO)==(BOOT_ACTIVE_LOW?LOW:HIGH);}void setup(){Serial.begin(115200);pinMode(PIN_BOOT_AUTO,INPUT_PULLUP);Serial.println("Hold BOOT for 5 seconds to erase NVS and restart.");}
void loop(){static uint32_t start=0;if(pressed()){if(!start)start=millis();uint32_t held=millis()-start;Serial.printf("Holding: %.1fs / %.1fs\r",held/1000.0f,FACTORY_RESET_HOLD_MS/1000.0f);if(held>=FACTORY_RESET_HOLD_MS){Serial.println("\nErasing NVS...");nvs_flash_deinit();nvs_flash_erase();delay(500);ESP.restart();}}else{start=0;}delay(50);}
