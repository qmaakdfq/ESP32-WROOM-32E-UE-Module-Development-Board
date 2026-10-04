#include <Arduino.h>
#include <esp_arduino_version.h>
#include "pins.h"
#include "config.h"
#if ESP_ARDUINO_VERSION_MAJOR >= 3
static bool pwmStart(){return ledcAttach(PIN_AUDIO_PWM,AUDIO_PWM_CARRIER_HZ,AUDIO_PWM_BITS);}static void pwmTone(uint32_t f){ledcWriteTone(PIN_AUDIO_PWM,f);}static void pwmStop(){ledcWriteTone(PIN_AUDIO_PWM,0);ledcWrite(PIN_AUDIO_PWM,0);ledcDetach(PIN_AUDIO_PWM);}
#else
static constexpr int CH=0;static bool pwmStart(){ledcSetup(CH,AUDIO_PWM_CARRIER_HZ,AUDIO_PWM_BITS);ledcAttachPin(PIN_AUDIO_PWM,CH);return true;}static void pwmTone(uint32_t f){ledcWriteTone(CH,f);}static void pwmStop(){ledcWriteTone(CH,0);ledcWrite(CH,0);ledcDetachPin(PIN_AUDIO_PWM);}
#endif
static void amp(bool on){digitalWrite(PIN_AMP_EN,on==AMP_EN_ACTIVE_HIGH?HIGH:LOW);} 
void setup(){Serial.begin(115200);pinMode(PIN_AMP_EN,OUTPUT);amp(false);pinMode(PIN_AUDIO_PWM,OUTPUT);digitalWrite(PIN_AUDIO_PWM,LOW);if(!pwmStart()){Serial.println("PWM init FAIL");return;}amp(true);Serial.println("1kHz / 2s");pwmTone(1000);delay(2000);Serial.println("Sweep 200Hz -> 4kHz");for(int f=200;f<=4000;f+=30){pwmTone(f);delay(12);}amp(false);pwmStop();pinMode(PIN_AUDIO_PWM,OUTPUT);digitalWrite(PIN_AUDIO_PWM,LOW);Serial.println("Audio test done; PWM stopped.");}
void loop(){delay(1000);}
