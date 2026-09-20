#ifndef SYSINFOAPP_H
#define SYSINFOAPP_H

#include "App.h"
#include <WiFi.h>

class SysInfoApp : public App {
public:
  SysInfoApp() : App("Systeme", 0x07FF) {}

  void draw() override {
    tft->fillScreen(0x0841);

    tft->setTextColor(TFT_WHITE, 0x0841);
    tft->setTextSize(2);
    tft->setCursor(15, 35);
    tft->print("Systeme");

    tft->setTextSize(1);
    int y = 80;

    auto drawLine = [&](String label, String value) {
      tft->setTextColor(0x7BEF, 0x0841);
      tft->setCursor(15, y);
      tft->print(label);
      tft->setTextColor(0x07FF, 0x0841);
      tft->setCursor(120, y);
      tft->print(value);
      y += 25;
    };

    drawLine("Chip:", ESP.getChipModel());
    drawLine("Freq:", String(ESP.getCpuFreqMHz()) + " MHz");
    drawLine("Flash:", String(ESP.getFlashChipSize() / 1024 / 1024) + " MB");
    drawLine("Heap:", String(ESP.getFreeHeap() / 1024) + " KB libres");
    drawLine("PSRAM:", String(ESP.getPsramSize() / 1024 / 1024) + " MB");
    drawLine("SDK:", ESP.getSdkVersion());
    drawLine("WiFi:", WiFi.status() == WL_CONNECTED ? "OK" : "Off");
    if (WiFi.status() == WL_CONNECTED) {
      drawLine("IP:", WiFi.localIP().toString());
      drawLine("RSSI:", String(WiFi.RSSI()) + " dBm");
    }
    drawLine("Uptime:", String(millis() / 1000) + "s");
  }

  void update() override {
    static unsigned long last = 0;
    if (millis() - last > 5000) {
      last = millis();
      draw();
    }
  }
};

App* createSysInfo() { return new SysInfoApp(); }
#endif