#ifndef ALARMAPP_H
#define ALARMAPP_H

#include "App.h"

class AlarmApp : public App {
private:
  int tab = 0;  // 0=Chrono, 1=Minuteur
  unsigned long startTime = 0;
  unsigned long elapsed = 0;
  bool running = false;

public:
  AlarmApp() : App("Reveil", 0xF800) {}

  void draw() override {
    tft->fillScreen(0x0841);

    // Onglets
    tft->fillRoundRect(10, 30, 105, 30, 8, tab == 0 ? 0x07E0 : 0x4208);
    tft->fillRoundRect(125, 30, 105, 30, 8, tab == 1 ? 0x07E0 : 0x4208);
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(1);
    tft->setCursor(40, 42);
    tft->print("CHRONO");
    tft->setCursor(155, 42);
    tft->print("MINUTEUR");

    // Affichage
    tft->fillRoundRect(20, 90, 200, 80, 12, 0x18C3);
    tft->setTextColor(0x07FF, 0x18C3);
    tft->setTextSize(4);
    tft->setCursor(40, 115);

    unsigned long t = running ? (elapsed + (millis() - startTime) / 1000) : elapsed;
    char buf[12];
    sprintf(buf, "%02d:%02d:%02d", (int)(t / 3600), (int)((t / 60) % 60), (int)(t % 60));
    tft->print(buf);

    // Boutons
    int btnColor = running ? 0xF800 : 0x07E0;
    tft->fillRoundRect(40, 210, 160, 50, 12, btnColor);
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(2);
    tft->setCursor(85, 225);
    tft->print(running ? "STOP" : "START");

    // Reset
    tft->fillRoundRect(40, 270, 160, 30, 8, 0x4208);
    tft->setTextSize(1);
    tft->setCursor(105, 280);
    tft->print("RESET");
  }

  void handleTouch(int x, int y) override {
    if (y >= 30 && y <= 60) {
      tab = (x < 120) ? 0 : 1;
      draw();
    }
    if (y >= 210 && y <= 260) {
      if (running) {
        elapsed += (millis() - startTime) / 1000;
        running = false;
      } else {
        startTime = millis();
        running = true;
      }
      draw();
    }
    if (y >= 270 && y <= 300) {
      elapsed = 0;
      running = false;
      draw();
    }
  }

  void update() override {
    if (running && millis() - startTime > 1000) {
      draw();
      startTime = millis() - (millis() - startTime) % 1000;
    }
  }
};

App* createAlarm() { return new AlarmApp(); }
#endif