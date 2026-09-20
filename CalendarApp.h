#ifndef CALENDARAPP_H
#define CALENDARAPP_H

#include "App.h"

class CalendarApp : public App {
private:
  int month = 9;
  int year = 2025;

public:
  CalendarApp() : App("Calendrier", 0x07E0) {}

  void draw() override {
    tft->fillScreen(0x0841);
    tft->setTextColor(TFT_WHITE, 0x0841);
    tft->setTextSize(2);
    tft->setCursor(15, 35);
    tft->print("Septembre");

    tft->setTextSize(1);
    tft->setTextColor(0x7BEF, 0x0841);
    tft->setCursor(180, 42);
    tft->print("2025");

    // Jours de la semaine
    const char* days[] = {"L","M","M","J","V","S","D"};
    for (int i = 0; i < 7; i++) {
      tft->setCursor(15 + i * 30, 70);
      tft->setTextColor(0x07FF, 0x0841);
      tft->print(days[i]);
    }

    // Grille
    int firstDay = 1; // Sept 2025 commence un lundi
    for (int d = 1; d <= 30; d++) {
      int col = (d + firstDay - 2) % 7;
      int row = (d + firstDay - 2) / 7;
      int x = 15 + col * 30;
      int y = 90 + row * 30;

      tft->drawRoundRect(x, y, 26, 26, 4, 0x4208);
      tft->setTextColor(TFT_WHITE, 0x0841);
      tft->setTextSize(1);
      tft->setCursor(x + 10, y + 10);
      tft->print(d);

      if (d == 19) {
        tft->fillRoundRect(x + 1, y + 1, 24, 24, 3, 0x07E0);
      }
    }
  }
};

App* createCalendar() { return new CalendarApp(); }
#endif