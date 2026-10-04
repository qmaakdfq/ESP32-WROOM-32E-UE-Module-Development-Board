#include <Arduino.h>
#include "pins.h"
#include "config.h"
void setup(){
  Serial.begin(115200); delay(300);
  Serial.println("ZLX-ESP32-1 / ESP32-2432S0248R-PLUS");
  Serial.printf("LCD: SCK%d MISO%d MOSI%d CS%d DC%d BL%d RST%d, rotation=%u, %dx%d, %luHz\n",PIN_LCD_SCK,PIN_LCD_MISO,PIN_LCD_MOSI,PIN_LCD_CS,PIN_LCD_DC,PIN_LCD_BL,PIN_LCD_RST,LCD_ROTATION,LCD_WIDTH,LCD_HEIGHT,(unsigned long)LCD_SPI_HZ);
  Serial.printf("Touch: SCK%d DIN%d DOUT%d CS%d IRQ%d, swap=%d invertX=%d invertY=%d rawX=%ld..%ld rawY=%ld..%ld\n",PIN_RTP_SCK,PIN_RTP_DIN,PIN_RTP_DOUT,PIN_RTP_CS,PIN_RTP_IRQ,TOUCH_SWAP_XY,TOUCH_INVERT_X,TOUCH_INVERT_Y,(long)TOUCH_RAW_X_MIN,(long)TOUCH_RAW_X_MAX,(long)TOUCH_RAW_Y_MIN,(long)TOUCH_RAW_Y_MAX);
  Serial.printf("TF: SCK%d MISO%d MOSI%d CS%d @ %luHz\n",PIN_SD_SCK,PIN_SD_MISO,PIN_SD_MOSI,PIN_SD_CS,(unsigned long)SD_SPI_HZ);
  Serial.printf("RGB: R=%d G=%d B=%d activeLow=%d\n",PIN_RGB_R,PIN_RGB_G,PIN_RGB_B,RGB_ACTIVE_LOW);
  Serial.printf("Audio: PWM=%d AMP_EN=%d activeHigh=%d\n",PIN_AUDIO_PWM,PIN_AMP_EN,AMP_EN_ACTIVE_HIGH);
  Serial.printf("Battery ADC: GPIO%d divider=%.2f calibration=%.3f\n",PIN_BAT_ADC,BAT_ADC_DIVIDER_RATIO,BAT_ADC_CALIBRATION);
  Serial.printf("GPS: RX GPIO%d @ %lu baud; BOOT GPIO%d activeLow=%d\n",PIN_GPS_RX,(unsigned long)GPS_DEFAULT_BAUD,PIN_BOOT_AUTO,BOOT_ACTIVE_LOW);
}
void loop(){delay(1000);}
