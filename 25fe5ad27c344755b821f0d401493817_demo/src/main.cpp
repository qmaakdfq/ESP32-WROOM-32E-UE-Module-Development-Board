#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "AudioTools.h"
#include "BluetoothA2DPSink.h"

// =====================
// 音频配置
// =====================
#define AUDIO_EN 4
#define AUDIO_EN_LEVEL LOW

// GPIO26 / DAC2 -> 8002B AUDIO_IN
AnalogAudioStream out;
BluetoothA2DPSink a2dp_sink(out);

// =====================
// 屏幕配置
// =====================
#define SCREEN_W 240
#define SCREEN_H 320

TFT_eSPI tft = TFT_eSPI();

static lv_disp_draw_buf_t draw_buf;
// LVGL 绘图缓冲区：原来是 SCREEN_W * 40，和 A2DP/AudioTools 同用时 DRAM 容易超限
// 10 行缓冲足够刷新，能明显降低 .dram0.bss 占用
static lv_color_t buf1[SCREEN_W * 10];

// =====================
// UI 对象
// =====================
static lv_obj_t *label_time;
static lv_obj_t *label_level;
static lv_obj_t *label_info;
// 不要叫 bars：ESP32 Arduino 内部 libpp.a 里已经有同名符号，会导致 multiple definition of `bars`
static lv_obj_t *ui_bars[20];
static lv_obj_t *arc_level;

// =====================
// 音频电平数据
// =====================
static volatile uint32_t audioLevel = 0;
static volatile uint32_t audioPackets = 0;

static uint32_t lastAmpKeepMs = 0;

// =====================
// LVGL 显示刷新
// =====================
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)color_p, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

// =====================
// 音频数据回调
// 只用于音频跳动显示，不改变原来的蓝牙播放输出
// =====================
void audio_raw_callback(const uint8_t *data, uint32_t len) {
  audioPackets = audioPackets + 1;

  const int16_t *samples = (const int16_t *)data;
  uint32_t sampleCount = len / 2;

  uint32_t acc = 0;
  uint32_t n = 0;

  // 抽样计算，避免回调太重
  for (uint32_t i = 0; i < sampleCount; i += 32) {
    int32_t s = samples[i];
    if (s < 0) s = -s;
    acc += s;
    n++;
  }

  if (n > 0) {
    uint32_t avg = acc / n;
    uint32_t level = avg * 100UL / 32768UL;
    if (level > 100) level = 100;

    // 平滑
    audioLevel = (audioLevel * 3 + level) / 4;
  }
}

// =====================
// 运行时间
// =====================
String getRunTime() {
  uint32_t sec = millis() / 1000;
  uint32_t h = sec / 3600;
  uint32_t m = (sec % 3600) / 60;
  uint32_t s = sec % 60;

  char buf[20];
  snprintf(buf, sizeof(buf), "%02lu:%02lu:%02lu",
           (unsigned long)h,
           (unsigned long)m,
           (unsigned long)s);

  return String(buf);
}

// =====================
// UI 样式
// =====================
static lv_style_t style_bg;
static lv_style_t style_card;
static lv_style_t style_title;
static lv_style_t style_small;
static lv_style_t style_value;

lv_obj_t *makeLabel(lv_obj_t *parent, const char *txt, int x, int y, lv_style_t *style) {
  lv_obj_t *label = lv_label_create(parent);
  lv_label_set_text(label, txt);
  lv_obj_add_style(label, style, 0);
  lv_obj_set_pos(label, x, y);
  return label;
}

lv_obj_t *makeCard(lv_obj_t *parent, int x, int y, int w, int h) {
  lv_obj_t *card = lv_obj_create(parent);
  lv_obj_set_size(card, w, h);
  lv_obj_set_pos(card, x, y);
  lv_obj_add_style(card, &style_card, 0);
  lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);
  return card;
}

void buildUI() {
  lv_obj_t *scr = lv_scr_act();

  // 背景渐变
  lv_style_init(&style_bg);
  lv_style_set_bg_color(&style_bg, lv_color_hex(0x06111F));
  lv_style_set_bg_grad_color(&style_bg, lv_color_hex(0x123A5A));
  lv_style_set_bg_grad_dir(&style_bg, LV_GRAD_DIR_VER);
  lv_obj_add_style(scr, &style_bg, 0);

  // 卡片样式
  lv_style_init(&style_card);
  lv_style_set_radius(&style_card, 18);
  lv_style_set_bg_color(&style_card, lv_color_hex(0x101B2E));
  lv_style_set_bg_opa(&style_card, LV_OPA_90);
  lv_style_set_border_width(&style_card, 1);
  lv_style_set_border_color(&style_card, lv_color_hex(0x315C8A));
  lv_style_set_pad_all(&style_card, 12);
  lv_style_set_shadow_width(&style_card, 16);
  lv_style_set_shadow_color(&style_card, lv_color_hex(0x000000));
  lv_style_set_shadow_opa(&style_card, LV_OPA_40);

  // 标题
  lv_style_init(&style_title);
  lv_style_set_text_color(&style_title, lv_color_hex(0xFFFFFF));
  lv_style_set_text_font(&style_title, &lv_font_montserrat_24);

  // 小字
  lv_style_init(&style_small);
  lv_style_set_text_color(&style_small, lv_color_hex(0x91B8E6));
  lv_style_set_text_font(&style_small, &lv_font_montserrat_14);

  // 数值
  lv_style_init(&style_value);
  lv_style_set_text_color(&style_value, lv_color_hex(0xFFFFFF));
  lv_style_set_text_font(&style_value, &lv_font_montserrat_20);

  // 顶部标题
  makeLabel(scr, "ESP32 AUDIO", 18, 16, &style_title);
  makeLabel(scr, "Bluetooth Speaker", 20, 48, &style_small);

  // 蓝牙设备卡片
  lv_obj_t *card1 = makeCard(scr, 14, 76, 212, 70);
  makeLabel(card1, "DEVICE", 4, 0, &style_small);
  makeLabel(card1, "ESP32-AUDIO", 4, 28, &style_value);

  // 时间卡片
  lv_obj_t *card2 = makeCard(scr, 14, 158, 212, 62);
  makeLabel(card2, "RUN TIME", 4, 0, &style_small);
  label_time = makeLabel(card2, "00:00:00", 4, 26, &style_value);

  // 电平圆环卡片
  lv_obj_t *card3 = makeCard(scr, 14, 232, 86, 72);
  makeLabel(card3, "LEVEL", 4, 0, &style_small);

  arc_level = lv_arc_create(card3);
  lv_obj_set_size(arc_level, 54, 54);
  lv_obj_set_pos(arc_level, 14, 18);
  lv_arc_set_range(arc_level, 0, 100);
  lv_arc_set_value(arc_level, 0);
  lv_arc_set_bg_angles(arc_level, 135, 45);
  lv_obj_remove_style(arc_level, NULL, LV_PART_KNOB);
  lv_obj_clear_flag(arc_level, LV_OBJ_FLAG_CLICKABLE);

  lv_obj_set_style_arc_color(arc_level, lv_color_hex(0x26384F), LV_PART_MAIN);
  lv_obj_set_style_arc_color(arc_level, lv_color_hex(0x38D6FF), LV_PART_INDICATOR);
  lv_obj_set_style_arc_width(arc_level, 6, LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc_level, 6, LV_PART_INDICATOR);

  label_level = makeLabel(card3, "0", 33, 35, &style_small);

  // 音频柱状卡片
  lv_obj_t *card4 = makeCard(scr, 108, 232, 118, 72);
  makeLabel(card4, "VISUAL", 4, 0, &style_small);

  for (int i = 0; i < 20; i++) {
    ui_bars[i] = lv_bar_create(card4);
    lv_obj_set_size(ui_bars[i], 4, 42);
    lv_obj_set_pos(ui_bars[i], 6 + i * 5, 22);
    lv_bar_set_range(ui_bars[i], 0, 100);
    lv_bar_set_value(ui_bars[i], 3, LV_ANIM_OFF);

    lv_obj_set_style_bg_color(ui_bars[i], lv_color_hex(0x1B2B40), LV_PART_MAIN);
    lv_obj_set_style_radius(ui_bars[i], 3, LV_PART_MAIN);
    lv_obj_set_style_radius(ui_bars[i], 3, LV_PART_INDICATOR);

    if (i < 7) {
      lv_obj_set_style_bg_color(ui_bars[i], lv_color_hex(0x37D6FF), LV_PART_INDICATOR);
    } else if (i < 14) {
      lv_obj_set_style_bg_color(ui_bars[i], lv_color_hex(0x55F0A0), LV_PART_INDICATOR);
    } else {
      lv_obj_set_style_bg_color(ui_bars[i], lv_color_hex(0xFFC857), LV_PART_INDICATOR);
    }
  }

  // 底部信息
  label_info = makeLabel(scr, "DAC2 GPIO26  |  VOL 127  |  AMP LOW", 18, 306, &style_small);
}

// =====================
// UI 定时刷新
// =====================
void uiTimer(lv_timer_t *timer) {
  static uint32_t lastPackets = 0;
  static uint32_t noPacketCount = 0;

  uint32_t level = audioLevel;

  // 如果没有收到音乐数据，显示轻微待机动画
  if (audioPackets == lastPackets) {
    noPacketCount++;
    if (noPacketCount > 10) {
      level = 2 + (millis() / 200) % 4;
    }
  } else {
    noPacketCount = 0;
  }

  lastPackets = audioPackets;

  lv_label_set_text(label_time, getRunTime().c_str());

  char levelText[12];
  snprintf(levelText, sizeof(levelText), "%lu", (unsigned long)level);
  lv_label_set_text(label_level, levelText);

  lv_arc_set_value(arc_level, level);

  for (int i = 0; i < 20; i++) {
    int weight = 55 + ((i * 29) % 70);
    int v = level * weight / 100;

    if ((millis() / 80 + i) % 4 == 0) {
      v += 10;
    }

    if (v < 3) v = 3;
    if (v > 100) v = 100;

    lv_bar_set_value(ui_bars[i], v, LV_ANIM_OFF);
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("ESP32 Bluetooth Speaker + LVGL UI");
  Serial.println("Bluetooth name: ESP32-AUDIO");
  Serial.println("AUDIO_EN = GPIO4 LOW active");
  Serial.println("AUDIO_OUT = GPIO26 / DAC2");

  // LOW 打开 8002B
  pinMode(AUDIO_EN, OUTPUT);
  digitalWrite(AUDIO_EN, AUDIO_EN_LEVEL);
  delay(200);

  // 初始化屏幕
  tft.init();
  tft.setRotation(0);

#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);
#endif

  // 初始化 LVGL
  lv_init();

  lv_disp_draw_buf_init(&draw_buf, buf1, NULL, SCREEN_W * 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_W;
  disp_drv.ver_res = SCREEN_H;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  buildUI();

  // UI 刷新，50ms 一次
  lv_timer_create(uiTimer, 50, NULL);

  // 蓝牙音频
  a2dp_sink.set_mono_downmix(true);

  // 最大音量
  // 如果音乐滋滋/破音严重，把 127 改成 100 或 90
  a2dp_sink.set_volume(127);

  // 只用于音频跳动显示
  a2dp_sink.set_raw_stream_reader(audio_raw_callback);

  // 启动蓝牙
  a2dp_sink.start("ESP32-AUDIO");

  Serial.println("Bluetooth A2DP started.");
}

void loop() {
  static uint32_t lastTick = millis();

  uint32_t now = millis();
  uint32_t diff = now - lastTick;
  lastTick = now;

  lv_tick_inc(diff);
  lv_timer_handler();

  // 保持功放低电平使能
  if (millis() - lastAmpKeepMs > 1000) {
    lastAmpKeepMs = millis();
    digitalWrite(AUDIO_EN, AUDIO_EN_LEVEL);
  }

  delay(5);
}