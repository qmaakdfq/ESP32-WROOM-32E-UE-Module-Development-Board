#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

TFT_eSPI tft = TFT_eSPI();
SPIClass touchSPI(VSPI);
XPT2046_Touchscreen ts(ETOUCH_CS, TOUCH_IRQ);

// --- 状态变量 ---
uint16_t currentColor = TFT_WHITE; // 默认画笔颜色
int currentThickness = 2;          // 默认画笔粗细
int lastX = -1;
int lastY = -1;
bool wasTouched = false;

// --- UI 区域布局定义 (横屏 320x240) ---
const int toolY = 200;
const int btnSize = 40; 

// 去掉了黑色，换成了青色 (TFT_CYAN)，一共 5 种纯彩色
uint32_t colors[5] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW, TFT_CYAN};

// 清屏按钮坐标 (右上角)
const int clearX = 230, clearY = 0, clearW = 90, clearH = 40;

// 绘制整个 UI 界面
void drawUI() {
    // 1. 画底部颜色选择按钮 (5个彩色)
    for (int i = 0; i < 5; i++) {
        int x = i * btnSize;
        tft.fillRect(x, toolY, btnSize, btnSize, colors[i]);
        tft.drawRect(x, toolY, btnSize, btnSize, TFT_WHITE); // 加上白色边框
    }

    // 2. 画画笔粗细按钮 (底部右侧2个)
    // 细笔按钮
    tft.fillRect(220, toolY, btnSize, btnSize, TFT_DARKGREY);
    tft.drawRect(220, toolY, btnSize, btnSize, TFT_WHITE);
    tft.fillCircle(220 + 20, toolY + 20, 2, TFT_WHITE); // 细圆点指示

    // 粗笔按钮
    tft.fillRect(270, toolY, btnSize, btnSize, TFT_DARKGREY);
    tft.drawRect(270, toolY, btnSize, btnSize, TFT_WHITE);
    tft.fillCircle(270 + 20, toolY + 20, 6, TFT_WHITE); // 粗圆点指示

    // 3. 画清屏按钮 (右上角)
    tft.fillRect(clearX, clearY, clearW, clearH, TFT_RED);
    tft.drawRect(clearX, clearY, clearW, clearH, TFT_WHITE);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("CLEAR", clearX + (clearW / 2), clearY + (clearH / 2));
}

void setup() {
    Serial.begin(115200);

    // 初始化背光
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);

    // 初始化屏幕
    tft.init();
    tft.setRotation(1);
    tft.invertDisplay(false); 
    tft.fillScreen(TFT_BLACK);

    // 初始化触摸
    touchSPI.begin(TOUCH_CLK, TOUCH_MISO, TOUCH_MOSI, ETOUCH_CS);
    ts.begin(touchSPI);
    ts.setRotation(1); 

    drawUI();
    Serial.println("System Ready! ER button removed.");
}

void loop() {
    if (ts.touched()) {
        TS_Point p = ts.getPoint();
        
        // 坐标映射
        int x = map(p.x, 300, 3800, 0, 320);
        int y = map(p.y, 300, 3800, 0, 240);

        // 1. 判断清屏按钮
        if (x > clearX && y < (clearY + clearH)) {
            tft.fillScreen(TFT_BLACK);
            drawUI();
            wasTouched = false;
            delay(300); 
            return;
        }

        // 2. 判断底部工具栏
        if (y > toolY) {
            if (x < 200) {
                // 点击了前5个颜色块
                int colorIndex = x / btnSize; 
                currentColor = colors[colorIndex];
                Serial.printf("Color changed to index %d\n", colorIndex);
            }
            else if (x > 220 && x < 260) {
                // 细笔
                currentThickness = 2;
                Serial.println("Thickness: THIN");
            }
            else if (x > 270 && x < 310) {
                // 粗笔
                currentThickness = 6;
                Serial.println("Thickness: THICK");
            }
            
            wasTouched = false; 
            delay(150); 
            return;
        }

        // 3. 在画板区域画画
        if (y < toolY && !(x > clearX && y < clearY + clearH)) {
            if (wasTouched) {
                // 连线平滑插值算法
                int dx = abs(x - lastX);
                int dy = abs(y - lastY);
                int steps = max(dx, dy); 
                
                if (steps == 0) steps = 1;
                
                for (int i = 0; i <= steps; i++) {
                    int cx = lastX + (x - lastX) * i / steps;
                    int cy = lastY + (y - lastY) * i / steps;
                    tft.fillCircle(cx, cy, currentThickness, currentColor);
                }
            } else {
                tft.fillCircle(x, y, currentThickness, currentColor);
            }
            
            lastX = x;
            lastY = y;
            wasTouched = true;
        }
    } else {
        wasTouched = false;
    }
}