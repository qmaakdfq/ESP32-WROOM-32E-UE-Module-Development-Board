#include <Arduino.h>
#include <TFT_eSPI.h>
#include "driver/gpio.h"
#include "pins.h"
#include "config.h"
TFT_eSPI tft; HardwareSerial gps(2); char buf[160];size_t len=0;String line1="Waiting for NMEA...",line2="",line3="";
static double coord(const char* v,const char* h){double r=atof(v);int d=int(r/100);double o=d+(r-d*100)/60.0;if(*h=='S'||*h=='W')o=-o;return o;}
static void draw(){tft.fillRect(0,40,320,190,TFT_BLACK);tft.setTextColor(TFT_WHITE,TFT_BLACK);tft.drawString(line1,8,55);tft.drawString(line2,8,85);tft.drawString(line3,8,115);}
static void parse(char* s){char* f[20]={};int n=1;f[0]=s;for(char* p=s;*p&&n<20;p++)if(*p==','){*p=0;f[n++]=p+1;}size_t L=strlen(f[0]);if(L<3)return;const char* type=f[0]+L-3;if(!strcmp(type,"GGA")&&n>9){int q=atoi(f[6]);line1=String("GGA Fix: ")+(q>0?"YES":"NO")+" Satellites: "+f[7];if(q>0&&*f[2]&&*f[4]){line2="Lat: "+String(coord(f[2],f[3]),6);line3="Lon: "+String(coord(f[4],f[5]),6);}}else if(!strcmp(type,"RMC")&&n>6){bool ok=f[2]&&f[2][0]=='A';line1=String("RMC Fix: ")+(ok?"YES":"NO");if(ok&&*f[3]&&*f[5]){line2="Lat: "+String(coord(f[3],f[4]),6);line3="Lon: "+String(coord(f[5],f[6]),6);}}draw();}
void setup(){pinMode(PIN_LCD_BL,OUTPUT);digitalWrite(PIN_LCD_BL,LCD_BL_ACTIVE_HIGH?HIGH:LOW);tft.init();tft.setRotation(LCD_ROTATION);tft.invertDisplay(LCD_INVERT);tft.fillScreen(TFT_BLACK);tft.setTextColor(TFT_CYAN,TFT_BLACK);tft.setTextSize(2);tft.drawString("GT-U7 GPS",8,8);tft.setTextSize(1);draw();Serial.begin(115200);Serial.println("Releasing UART0, then GPS uses GPIO1.");Serial.flush();delay(100);Serial.end();gpio_reset_pin((gpio_num_t)PIN_GPS_RX);gps.setRxBufferSize(2048);gps.begin(GPS_DEFAULT_BAUD,SERIAL_8N1,PIN_GPS_RX,-1);}
void loop(){while(gps.available()){uint8_t b=gps.read();if(b=='$'){len=0;buf[len++]='$';continue;}if(!len)continue;if(b=='\n'){if(len>=6){buf[len]=0;parse(buf);}len=0;continue;}if(b=='\r')continue;if(b<0x20||b>0x7E){len=0;continue;}if(len<sizeof(buf)-1)buf[len++]=char(b);else len=0;}delay(2);}
