#ifndef CONVERTERAPP_H
#define CONVERTERAPP_H

#include "App.h"

class ConverterApp : public App {
private:
  int category = 0;  // 0=temp, 1=long, 2=poids
  float value = 100;

  const char* catNames[3] = {"Temp", "Long", "Poids"};

public:
  ConverterApp() : App("Convert", 0xFFE0) {}

  void draw() override {
    tft->fillScreen(0x0841);

    // Onglets
    for (int i = 0; i < 3; i++) {
      tft->fillRoundRect(10 + i * 75, 30, 70, 30, 6, category == i ? 0x07E0 : 0x4208);
      tft->setTextColor(TFT_WHITE, category == i ? 0x07E0 : 0x4208);
      tft->setTextSize(1);
      tft->setCursor(20 + i * 75, 42);
      tft->print(catNames[i]);
    }

    // Valeur
    tft->setTextColor(TFT_WHITE, 0x0841);
    tft->setTextSize(1);
    tft->setCursor(15, 90);
    tft->print("Valeur:");

    tft->fillRoundRect(15, 110, 210, 50, 8, 0x18C3);
    tft->setTextColor(0x07FF, 0x18C3);
    tft->setTextSize(3);
    tft->setCursor(30, 122);
    tft->print(value, 1);

    // Résultat
    tft->setTextColor(TFT_WHITE, 0x0841);
    tft->setTextSize(1);
    tft->setCursor(15, 180);
    tft->print("Resultat:");

    tft->fillRoundRect(15, 200, 210, 50, 8, 0x07E0);
    tft->setTextColor(TFT_BLACK, 0x07E0);
    tft->setTextSize(3);
    tft->setCursor(30, 212);

    float result = 0;
    if (category == 0) {
      // C → F
      result = value * 9.0 / 5.0 + 32;
    } else if (category == 1) {
      // cm → inch
      result = value / 2.54;
    } else {
      // kg → lbs
      result = value * 2.20462;
    }
    tft->print(result, 1);

    // Boutons +/- et catégorie
    tft->fillRoundRect(15, 265, 60, 40, 8, 0xF800);
    tft->setTextColor(TFT_WHITE, 0xF800);
    tft->setTextSize(2);
    tft->setCursor(38, 275);
    tft->print("-");

    tft->fillRoundRect(90, 265, 60, 40, 8, 0x07E0);
    tft->setTextColor(TFT_WHITE, 0x07E0);
    tft->setCursor(110, 275);
    tft->print("+");

    tft->fillRoundRect(165, 265, 60, 40, 8, 0x4208);
    tft->setTextColor(TFT_WHITE, 0x4208);
    tft->setTextSize(1);
    tft->setCursor(180, 280);
    tft->print("NEXT");
  }

  void handleTouch(int x, int y) override {
    if (y >= 30 && y <= 60) {
      category = x / 75;
      category = constrain(category, 0, 2);
      draw();
      return;
    }
    if (y >= 265 && y <= 305) {
      if (x < 75) value = max(0.0f, value - 10);
      else if (x < 150) value += 10;
      else category = (category + 1) % 3;
      draw();
    }
  }
};

App* createConverter() { return new ConverterApp(); }
#endif