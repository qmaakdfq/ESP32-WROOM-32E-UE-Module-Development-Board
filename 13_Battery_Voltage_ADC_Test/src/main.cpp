#include <Arduino.h>
#include "pins.h"
#include "config.h"
static float sample(uint16_t& raw,uint32_t& mv){analogRead(PIN_BAT_ADC);delayMicroseconds(250);uint32_t rs=0;uint64_t ms=0;for(uint16_t i=0;i<BAT_ADC_SAMPLE_COUNT;i++){rs+=analogRead(PIN_BAT_ADC);ms+=analogReadMilliVolts(PIN_BAT_ADC);delayMicroseconds(BAT_ADC_SAMPLE_DELAY_US);}raw=rs/BAT_ADC_SAMPLE_COUNT;mv=ms/BAT_ADC_SAMPLE_COUNT;return (mv/1000.0f)*BAT_ADC_DIVIDER_RATIO*BAT_ADC_CALIBRATION;}
void setup(){Serial.begin(115200);pinMode(PIN_BAT_ADC,INPUT);analogReadResolution(12);analogSetPinAttenuation(PIN_BAT_ADC,ADC_11db);}
void loop(){uint16_t raw;uint32_t mv;float v=sample(raw,mv);Serial.printf("ADC=%u GPIO34=%lumV Battery=%.3fV Present=%s\n",raw,(unsigned long)mv,v,v>=BATTERY_PRESENT_VOLTAGE?"YES":"NO");delay(BATTERY_SERIAL_INTERVAL_MS);}
