#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include <algorithm>
void setup(){Serial.begin(115200);delay(300);WiFi.mode(WIFI_STA);WiFi.disconnect(false,false);delay(100);int n=WiFi.scanNetworks(false,true,false,300,0);Serial.printf("Found %d networks\n",n);if(n>0){std::vector<int> o(n);for(int i=0;i<n;i++)o[i]=i;std::sort(o.begin(),o.end(),[](int a,int b){return WiFi.RSSI(a)>WiFi.RSSI(b);});for(int r=0;r<n;r++){int i=o[r];String s=WiFi.SSID(i);if(!s.length())s="<Hidden>";Serial.printf("%2d. %4d dBm CH%2d %s\n",r+1,WiFi.RSSI(i),WiFi.channel(i),s.c_str());}}WiFi.scanDelete();}
void loop(){delay(1000);}
