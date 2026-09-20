#ifndef DICEAPP_H
#define DICEAPP_H

#include "App.h"

class DiceApp : public App {
private:
  int value = 1;
  int rolls = 0;

  void drawDie(int x, int y, int size, int v) {
    tft->fillRoundRect(x, y, size, size, 15, TFT_WHITE);
    tft->drawRoundRect(x, y, size, size, 15, 0x7BEF);

    int dotSize = size / 10;
    int c = size / 2;

    auto dot = [&](int dx, int dy) {
      tft->fillCircle(x + c + dx, y + c + dy, dotSize, TFT_BLACK);
    };

    int o = size / 4;

    switch(v) {
      case 1: dot(0, 0); break;
      case 2: dot(-o, -o); dot(o, o); break;
      case 3: dot(-o, -o); dot(0, 0); dot(o, o); break;
      case 4: dot(-o, -o); dot(o, -o); dot(-o, o); dot(o, o); break;
      case 5: dot(-o, -o); dot(o, -o); dot(0, 0); dot(-o, o); dot(o, o); break;
      case 6: dot(-o, -o); dot(o, -o); dot(-o, 0); dot(o, 0); dot(-o, o); dot(o, o); break;
    }
  }

public:
  DiceApp() : App("Des", 0xFFE0) {}

  void draw() override {
    tft->fillScreen(0x0841);

    tft->setTextColor(TFT_WHITE, 0x0841);
    tft->setTextSize(2);
    tft->setCursor(70, 40);
    tft->print("Lancer de des");

    // Dé
    drawDie(70, 100, 100, value);

    tft->setTextColor(0x7BEF, 0x0841);
    tft->setTextSize(1);
    tft->setCursor(85, 220);
    tft->print("Lancers: " + String(rolls));

    // Bouton
    tft->fillRoundRect(40, 250, 160, 50, 12, 0x07E0);
    tft->setTextColor(TFT_WHITE, 0x07E0);
    tft->setTextSize(2);
    tft->setCursor(75, 265);
    tft->print("LANCER");
  }

  void handleTouch(int x, int y) override {
    if (y >= 250 && y <= 300) {
      // Animation
      for (int i = 0; i < 10; i++) {
        value = random(1, 7);
        draw();
        delay(50);
      }
      value = random(1, 7);
      rolls++;
      draw();
    }
  }
};

App* createDice() { return new DiceApp(); }
#endif