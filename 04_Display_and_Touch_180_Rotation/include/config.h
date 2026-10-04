#pragma once
#include <Arduino.h>

// 2.8 英寸 ST7789 物理分辨率为 240x320，程序旋转为横屏 320x240。
static constexpr uint8_t LCD_ROTATION = 1;
static constexpr int LCD_WIDTH = 320;
static constexpr int LCD_HEIGHT = 240;
static constexpr uint32_t LCD_SPI_HZ = 40000000;
// TF卡与LCD 独立 SPI（LCD VSPI / SD HSPI）。工厂测试固定使用4MHz 保证兼容性。
static constexpr uint32_t SD_SPI_HZ = 4000000;  // 4MHz，与 HaleHound/Marauder 可工作固件一致
static constexpr uint32_t SD_OPERATION_TIMEOUT_MS = 3000;
static constexpr size_t SD_MAX_TEST_BYTES = 512;
static constexpr bool LCD_INVERT = false;   // 不开启反色

// XPT2046 独立软件 SPI。
static constexpr uint32_t TOUCH_SPI_HZ = 2000000;
static constexpr uint16_t TOUCH_MIN_PRESSURE = 70;
static constexpr uint32_t TOUCH_RELEASE_DEBOUNCE_MS = 45;

// 固定触摸映射：不显示校准页面、不保存校准数据，开机直接使用。
// 这是常见 2.8 寸横屏方向的默认值。如触摸方向与实物不同，只改这里。
static constexpr bool TOUCH_SWAP_XY = true;
static constexpr bool TOUCH_INVERT_X = false;
static constexpr bool TOUCH_INVERT_Y = false;  // 修正实物触摸上下反向
static constexpr int32_t TOUCH_RAW_X_MIN = 220;
static constexpr int32_t TOUCH_RAW_X_MAX = 3880;
static constexpr int32_t TOUCH_RAW_Y_MIN = 220;
static constexpr int32_t TOUCH_RAW_Y_MAX = 3880;

static constexpr uint32_t FACTORY_RESET_HOLD_MS = 5000;
static constexpr uint32_t AUDIO_PWM_CARRIER_HZ = 62500;
static constexpr uint8_t AUDIO_PWM_BITS = 8;
static constexpr uint32_t GPS_DEFAULT_BAUD = 9600;
// GPS parsing runs continuously. During acquisition use a quieter 3s UI refresh
// cadence, then 1s after FIX. FIX detection itself is immediate and independent
// of the display timer; per-line caching sends only changed text to the TFT.
static constexpr uint32_t GPS_UI_SEARCH_REFRESH_MS = 3000;
static constexpr uint32_t GPS_UI_FIXED_REFRESH_MS = 1000;

// GPIO34 / BAT_ADC 电池电压检测。
// BAT+ → R16 100kΩ → GPIO34 → R17 100kΩ → GND，C21/C22 100nF 滤波。
static constexpr float BAT_ADC_R_TOP_OHM = 100000.0f;
static constexpr float BAT_ADC_R_BOTTOM_OHM = 100000.0f;
static constexpr float BAT_ADC_DIVIDER_RATIO = (BAT_ADC_R_TOP_OHM + BAT_ADC_R_BOTTOM_OHM) / BAT_ADC_R_BOTTOM_OHM; // 2.0
// 用万用表测得真实电池电压后，可设置为：真实电压 ÷ 屏幕显示电压。
static constexpr float BAT_ADC_CALIBRATION = 1.000f;
static constexpr uint16_t BAT_ADC_SAMPLE_COUNT = 32;
static constexpr uint16_t BAT_ADC_SAMPLE_DELAY_US = 250;
static constexpr uint32_t BATTERY_UI_REFRESH_MS = 1000;
static constexpr uint32_t BATTERY_SERIAL_INTERVAL_MS = 2000;
static constexpr float BATTERY_PRESENT_VOLTAGE = 2.50f;
