#ifndef BOOKAPP_H
#define BOOKAPP_H

#include "App.h"

class BookApp : public App {
private:
  int page = 0;
  const char* pages[4] = {
    "Bienvenue dans CheapyOS!\n\nCeci est un livre numerique. Touchez la fleche droite pour continuer la lecture.",
    "CheapyOS est un systeme d'exploitation concu pour fonctionner sur des microcontroleurs ESP32.",
    "Il propose une interface tactile, des applications variees, et un design moderne inspire d'Android.",
    "Vous pouvez ajouter vos propres textes dans le code source pour lire ce que vous voulez. Bonne lecture !"
  };

public:
  BookApp() : App("Livre", 0xFFE0) {}

  void draw() override {
    tft->fillScreen(0xF7BE);

    // Bordure
    tft->drawRect(5, 30, 230, 220, 0x0000);
    tft->drawRect(6, 31, 228, 218, 0x0000);

    // Titre
    tft->setTextColor(0x0000, 0xF7BE);
    tft->setTextSize(1);
    tft->setCursor(20, 45);
    tft->print("CheapyOS - Le livre");

    // Texte
    tft->setTextColor(0x0000, 0xF7BE);
    tft->setTextSize(1);
    tft->setCursor(20, 70);

    // Afficher la page avec retour à la ligne
    String text = pages[page];
    int x = 20, y = 70;
    for (int i = 0; i < text.length(); i++) {
      char c = text[i];
      if (c == '\n') {
        y += 12;
        x = 20;
        tft->setCursor(x, y);
      } else {
        tft->write(c);
        if (tft->getCursorX() > 210) {
          y += 12;
          x = 20;
          tft->setCursor(x, y);
        }
      }
      if (y > 230) break;
    }

    // Navigation
    if (page > 0) {
      tft->fillRoundRect(15, 270, 60, 35, 8, 0x07E0);
      tft->fillTriangle(55, 287, 35, 277, 35, 297, TFT_WHITE);
    }
    if (page < 3) {
      tft->fillRoundRect(165, 270, 60, 35, 8, 0x07E0);
      tft->fillTriangle(185, 287, 205, 277, 205, 297, TFT_WHITE);
    }

    // Page
    tft->setTextColor(0x0000, 0xF7BE);
    tft->setTextSize(1);
    tft->setCursor(100, 285);
    tft->print(page + 1);
    tft->print("/4");
  }

  void handleTouch(int x, int y) override {
    if (y >= 270 && y <= 305) {
      if (x < 80 && page > 0) { page--; draw(); }
      if (x > 160 && page < 3) { page++; draw(); }
    }
  }
};

App* createBook() { return new BookApp(); }
#endif