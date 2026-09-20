#include <ESP32Time.h>

class ClockApp : public App {
private:
  ESP32Time rtc;
  unsigned long lastUpdate = 0;
  
public:
  ClockApp() : App("Horloge", 0x001F) {}
  
  void draw() override {
    tft->fillScreen(TFT_BLACK);
    updateClock();
  }
  
  void updateClock() {
    tft->fillRect(0, 20, 320, 220, TFT_BLACK);
    
    char timeStr[10];
    sprintf(timeStr, "%02d:%02d:%02d", rtc.getHour(true), rtc.getMinute(), rtc.getSecond());
    
    tft->setTextColor(0x07FF, TFT_BLACK);
    tft->setTextSize(5);
    int tw = strlen(timeStr) * 30;
    tft->setCursor((320 - tw) / 2, 80);
    tft->print(timeStr);
    
    char dateStr[20];
    sprintf(dateStr, "%02d/%02d/%04d", rtc.getDay(), rtc.getMonth() + 1, rtc.getYear());
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setTextSize(2);
    tft->setCursor(100, 160);
    tft->print(dateStr);
  }
  
  void update() override {
    if (millis() - lastUpdate > 1000) {
      lastUpdate = millis();
      updateClock();
    }
  }
};

App* createClock() { return new ClockApp(); }