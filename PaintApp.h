#ifndef PAINTAPP_H
#define PAINTAPP_H

#include "App.h"

class PaintApp : public App {
private:
  uint16_t color = 0xF800;
  int brushSize = 3;
  int lastX = -1, lastY = -1;
  int colorIdx = 0;
  uint16_t colors[6] = {0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, TFT_WHITE};

public:
  PaintApp() : App("Dessin", 0xF81F) {}

  void draw() override {
    tft->fillScreen(TFT_WHITE);

    // Barre de couleurs en haut
    for (int i = 0; i < 6; i++) {
      tft->fillRoundRect(10 + i * 35, 30, 30, 30, 6, colors[i]);
      tft->drawRoundRect(10 + i * 35, 30, 30, 30, 6, colorIdx == i ? TFT_BLACK : 0x7BEF);
      if (colorIdx == i) {
        tft->drawRoundRect(9 + i * 35, 29, 32, 32, 7, TFT_BLACK);
      }
    }

    // Bouton gomme
    tft->fillRoundRect(220, 30, 15, 30, 4, 0x7BEF);
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(1);
    tft->setCursor(222, 42);
    tft->print("E");

    // Zone de dessin
    tft->drawRect(0, 65, 240, 220, 0x7BEF);
  }

  void handleTouch(int x, int y) override {
    // Sélection couleur
    if (y >= 30 && y <= 60) {
      for (int i = 0; i < 6; i++) {
        if (x >= 10 + i * 35 && x <= 40 + i * 35) {
          colorIdx = i;
          color = colors[i];
          draw();
          return;
        }
      }
      if (x >= 220) {
        color = TFT_WHITE;
        return;
      }
      return;
    }

    // Dessin
    if (y > 65 && y < 285) {
      if (lastX >= 0 && lastY >= 0) {
        tft->drawLine(lastX, lastY, x, y, color);
        tft->drawLine(lastX + 1, lastY, x + 1, y, color);
        tft->drawLine(lastX, lastY + 1, x, y + 1, color);
      }
      lastX = x;
      lastY = y;
    }
  }

  void exit() override {
    lastX = -1;
    lastY = -1;
  }
};

App* createPaint() { return new PaintApp(); }
#endif