#ifndef CAMERAAPP_H
#define CAMERAAPP_H

#include "App.h"

class CameraApp : public App {
private:
  int photos = 0;

public:
  CameraApp() : App("Camera", 0x7BEF) {}

  void draw() override {
    tft->fillScreen(TFT_BLACK);

    // Zone de prévisualisation
    tft->drawRect(15, 30, 210, 220, TFT_WHITE);
    for (int y = 30; y < 250; y += 10) {
      tft->drawFastHLine(15, y, 210, 0x2104);
    }
    for (int x = 15; x < 225; x += 10) {
      tft->drawFastVLine(x, 30, 220, 0x2104);
    }

    tft->setTextColor(0x7BEF, TFT_BLACK);
    tft->setTextSize(1);
    tft->setCursor(85, 140);
    tft->print("PAS DE CAMERA");

    // Bouton capture
    tft->fillCircle(120, 280, 22, TFT_WHITE);
    tft->fillCircle(120, 280, 18, 0xF800);
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(1);
    tft->setCursor(105, 260);
    tft->print("CAPTURE");

    // Compteur
    tft->setCursor(10, 260);
    tft->print("Photos: " + String(photos));
  }

  void handleTouch(int x, int y) override {
    int dx = x - 120;
    int dy = y - 280;
    if (dx*dx + dy*dy < 25*25) {
      photos++;
      // Effet flash
      tft->fillScreen(TFT_WHITE);
      delay(100);
      tft->fillScreen(TFT_BLACK);
      draw();
    }
  }
};

App* createCamera() { return new CameraApp(); }
#endif