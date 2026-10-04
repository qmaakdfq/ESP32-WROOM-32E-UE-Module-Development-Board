#pragma once
#include <Arduino.h>

// ZLX-ESP32-1 (32E/UE) GPIO 定义 — 基于 ESP32-2432S0248R-PLUS / ESP32-WROOM-32E
// 参考产品手册与 ESP32E_GPIO映射表

// LCD / ST7789 (HSPI，USE_HSPI_PORT)
static constexpr int PIN_LCD_SCK      = 14;
static constexpr int PIN_LCD_MISO     = 12;
static constexpr int PIN_LCD_MOSI     = 13;
static constexpr int PIN_LCD_CS       = 15;
static constexpr int PIN_LCD_DC       = 2;
static constexpr int PIN_LCD_BL       = 21;
// RST 接 EN，不占用 GPIO
static constexpr int PIN_LCD_RST      = -1;

// 触摸 / XPT2046 独立软件 SPI
static constexpr int PIN_RTP_SCK      = 25;
static constexpr int PIN_RTP_DIN      = 32;  // MOSI
static constexpr int PIN_RTP_DOUT     = 39;  // MISO (输入专用)
static constexpr int PIN_RTP_CS       = 33;
static constexpr int PIN_RTP_IRQ      = 36;  // 输入专用

// SD 卡 (VSPI / 全局 SPI，与 LCD 的 HSPI 分离)
static constexpr int PIN_SD_SCK       = 18;
static constexpr int PIN_SD_MISO      = 19;
static constexpr int PIN_SD_MOSI      = 23;
static constexpr int PIN_SD_CS        = 5;

// 板载 RGB 三色灯（低电平点亮）
// 实测：按红色按钮亮的是绿色，按绿色按钮亮的是红色 → R/G 对调
static constexpr int PIN_RGB_R        = 22;
static constexpr int PIN_RGB_G        = 17;
static constexpr int PIN_RGB_B        = 16;

// 电池电压 ADC（输入专用脚）
static constexpr int PIN_BAT_ADC      = 34;

// 音频
static constexpr int PIN_AUDIO_PWM    = 26;  // AUDIO_IN 复用为 PWM 输出
static constexpr int PIN_AMP_EN       = 4;   // AUDIO_EN

// GPS UART — 本板实测（HaleHound 固件确认）：GPS TX 接 GPIO1，9600
// 供电：GPS 模块 5V
//   GPS VCC  →  5V
//   GPS GND  →  GND
//   GPS TX   →  ESP32 GPIO1（MCU 接收）
//   GPS RX   →  不接
// GPIO1 默认是 UART0 TX（CH340），进 GPS 页必须 Serial.end() 才能改成 UART2 RX
// 禁止把 GPIO1 设成 OUTPUT；否则会顶掉 GPS TX 信号
static constexpr int PIN_GPS_RX       = 1;   // MCU RX ← GPS TX（实测）
static constexpr int PIN_GPS_TX       = -1;  // 不发送

// BOOT 按键（若板载无实体按键可忽略；保留兼容）
static constexpr int PIN_BOOT_AUTO    = 0;

// 电平有效性
static constexpr bool BOOT_ACTIVE_LOW      = true;
static constexpr bool AMP_EN_ACTIVE_HIGH   = true;
static constexpr bool RGB_ACTIVE_LOW       = true;   // 三通道均为低电平点亮
static constexpr bool LCD_BL_ACTIVE_HIGH   = true;

// 兼容旧宏（原绿灯改为 RGB）
static constexpr int PIN_GREEN_LED         = PIN_RGB_G;  // 兼容引用
static constexpr bool GREEN_LED_ACTIVE_LOW = true;
