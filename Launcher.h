#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <TFT_eSPI.h>
#include <WiFi.h>
#include "App.h"

extern App* createCalculator();
extern App* createSettings();
extern App* createClock();
extern App* createNotes();
extern App* createWiFi();
extern App* createMusic();
extern App* createGallery();
extern App* createWeather();
extern App* createGame();
extern App* createFiles();
extern App* createCalendar();
extern App* createAlarm();
extern App* createCamera();
extern App* createPaint();
extern App* createPong();
extern App* createTicTacToe();
extern App* createSysInfo();
extern App* createConverter();
extern App* createDice();
extern App* createBook();

class Launcher {
private:
  TFT_eSPI* tft;
  App* apps[20];
  int appCount = 20;
  int currentPage = 0;
  int iconsPerPage = 6;

  // ============================================================
  // FOND DÉGRADÉ + ÉTOILES
  // ============================================================
  void drawBackground() {
    for (int y = 0; y < 320; y++) {
      uint8_t r = map(y, 0, 320, 15, 40);
      uint8_t g = map(y, 0, 320, 10, 20);
      uint8_t b = map(y, 0, 320, 60, 100);
      tft->drawFastHLine(0, y, 240, tft->color565(r, g, b));
    }
    for (int i = 0; i < 60; i++) {
      int sx = random(0, 240);
      int sy = random(26, 320);
      uint8_t v = random(100, 255);
      tft->drawPixel(sx, sy, tft->color565(v, v, v));
    }
  }

  // ============================================================
  // WIDGET HEURE EN HAUT
  // ============================================================
  void drawTimeWidget() {
    tft->setTextColor(TFT_WHITE, tft->color565(20, 25, 50));
    tft->setTextSize(4);
    tft->setCursor(15, 32);

    unsigned long sec = millis() / 1000;
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", (sec / 60) % 24, sec % 60);
    tft->print(timeStr);

    tft->setTextSize(1);
    tft->setTextColor(tft->color565(180, 200, 255), tft->color565(20, 25, 50));
    tft->setCursor(15, 72);
    tft->print("Bonjour, User");
  }

  // ============================================================
  // ICÔNES DESSINÉES
  // ============================================================
  void drawIconSymbol(int x, int y, int idx, int size) {
    int cx = x + size / 2;
    int cy = y + size / 2;

    switch(idx) {
      case 0:
        tft->drawRect(cx-9, cy-11, 18, 22, TFT_WHITE);
        tft->drawRect(cx-7, cy-9, 14, 5, TFT_WHITE);
        for (int i = 0; i < 3; i++)
          for (int j = 0; j < 3; j++)
            tft->fillRect(cx-5 + i*5, cy-1 + j*5, 3, 3, TFT_WHITE);
        break;
      case 1:
        tft->drawCircle(cx, cy, 10, TFT_WHITE);
        tft->drawCircle(cx, cy, 4, TFT_WHITE);
        for (int i = 0; i < 8; i++) {
          float a = i * PI / 4;
          tft->fillCircle(cx + cos(a)*9, cy + sin(a)*9, 2, TFT_WHITE);
        }
        break;
      case 2:
        tft->drawCircle(cx, cy, 11, TFT_WHITE);
        tft->drawLine(cx, cy, cx, cy-7, TFT_WHITE);
        tft->drawLine(cx, cy, cx+5, cy+2, TFT_WHITE);
        tft->fillCircle(cx, cy, 2, TFT_WHITE);
        break;
      case 3:
        tft->drawRect(cx-9, cy-11, 18, 22, TFT_WHITE);
        for (int i = -6; i <= 4; i += 4)
          tft->drawFastHLine(cx-6, cy+i, 12, TFT_WHITE);
        break;
      case 4:
        for (int i = 1; i <= 3; i++)
          tft->drawCircleHelper(cx, cy+7, i*4, 0x0C, TFT_WHITE);
        tft->fillCircle(cx, cy+7, 2, TFT_WHITE);
        break;
      case 5:
        tft->fillCircle(cx-5, cy+6, 4, TFT_WHITE);
        tft->fillCircle(cx+6, cy+4, 4, TFT_WHITE);
        tft->drawLine(cx-1, cy+6, cx-1, cy-9, TFT_WHITE);
        tft->drawLine(cx+10, cy+4, cx+10, cy-12, TFT_WHITE);
        tft->drawLine(cx-1, cy-9, cx+10, cy-12, TFT_WHITE);
        break;
      case 6:
        tft->drawRect(cx-11, cy-8, 22, 16, TFT_WHITE);
        tft->fillCircle(cx-5, cy-3, 2, TFT_WHITE);
        tft->fillTriangle(cx-10, cy+7, cx-3, cy-1, cx+3, cy+7, TFT_WHITE);
        tft->fillTriangle(cx+2, cy+7, cx+6, cy+2, cx+10, cy+7, TFT_WHITE);
        break;
      case 7:
        tft->fillCircle(cx-2, cy-2, 6, TFT_WHITE);
        for (int i = 0; i < 8; i++) {
          float a = i * PI / 4;
          tft->drawLine(cx-2 + cos(a)*8, cy-2 + sin(a)*8,
                        cx-2 + cos(a)*11, cy-2 + sin(a)*11, TFT_WHITE);
        }
        break;
      case 8:
        tft->fillRect(cx-10, cy-8, 5, 5, TFT_WHITE);
        tft->fillRect(cx-5, cy-8, 5, 5, TFT_WHITE);
        tft->fillRect(cx, cy-8, 5, 5, TFT_WHITE);
        tft->fillRect(cx, cy-3, 5, 5, TFT_WHITE);
        tft->fillRect(cx, cy+2, 5, 5, TFT_WHITE);
        tft->fillCircle(cx-8, cy+6, 2, TFT_RED);
        break;
      case 9:
        tft->fillRect(cx-10, cy-5, 20, 15, TFT_WHITE);
        tft->fillRect(cx-10, cy-9, 9, 4, TFT_WHITE);
        tft->fillRect(cx-7, cy-2, 7, 9, tft->color565(255, 180, 0));
        break;
              case 10: // Calendrier
        tft->drawRect(cx-11, cy-10, 22, 20, TFT_WHITE);
        tft->drawFastHLine(cx-11, cy-5, 22, TFT_WHITE);
        tft->fillRect(cx-8, cy-3, 3, 3, TFT_WHITE);
        tft->fillRect(cx-3, cy-3, 3, 3, TFT_WHITE);
        tft->fillRect(cx+2, cy-3, 3, 3, TFT_WHITE);
        tft->fillRect(cx-8, cy+2, 3, 3, TFT_WHITE);
        tft->fillRect(cx-3, cy+2, 3, 3, TFT_WHITE);
        break;
      case 11: // Réveil
        tft->drawCircle(cx, cy, 12, TFT_WHITE);
        tft->drawLine(cx, cy, cx, cy-7, TFT_WHITE);
        tft->drawLine(cx, cy, cx+5, cy+2, TFT_WHITE);
        tft->fillCircle(cx-10, cy-10, 3, TFT_WHITE);
        tft->fillCircle(cx+10, cy-10, 3, TFT_WHITE);
        break;
      case 12: // Caméra
        tft->drawRoundRect(cx-11, cy-7, 22, 16, 3, TFT_WHITE);
        tft->fillCircle(cx, cy+1, 5, TFT_WHITE);
        tft->fillRect(cx-6, cy-10, 12, 4, TFT_WHITE);
        break;
      case 13: // Dessin
        tft->drawLine(cx-10, cy+8, cx+8, cy-10, TFT_WHITE);
        tft->drawLine(cx-9, cy+8, cx+9, cy-10, TFT_WHITE);
        tft->fillTriangle(cx-10, cy+8, cx-13, cy+11, cx-7, cy+11, TFT_WHITE);
        tft->fillCircle(cx+8, cy-10, 2, TFT_WHITE);
        break;
      case 14: // Pong
        tft->drawFastVLine(cx, cy-10, 20, TFT_WHITE);
        tft->fillRect(cx-11, cy-3, 3, 8, TFT_WHITE);
        tft->fillRect(cx+8, cy-5, 3, 8, TFT_WHITE);
        tft->fillCircle(cx-3, cy, 2, TFT_WHITE);
        break;
      case 15: // Morpion
        tft->drawFastVLine(cx-4, cy-11, 22, TFT_WHITE);
        tft->drawFastVLine(cx+4, cy-11, 22, TFT_WHITE);
        tft->drawFastHLine(cx-11, cy-4, 22, TFT_WHITE);
        tft->drawFastHLine(cx-11, cy+4, 22, TFT_WHITE);
        break;
      case 16: // Système
        tft->drawRect(cx-11, cy-9, 22, 16, TFT_WHITE);
        tft->drawFastHLine(cx-5, cy+10, 10, TFT_WHITE);
        tft->fillRect(cx-8, cy-6, 16, 2, TFT_WHITE);
        tft->fillRect(cx-8, cy-2, 10, 2, TFT_WHITE);
        tft->fillRect(cx-8, cy+2, 12, 2, TFT_WHITE);
        break;
      case 17: // Convertisseur
        tft->setTextColor(TFT_WHITE, apps[17]->iconColor);
        tft->setTextSize(2);
        tft->setCursor(cx-8, cy-8);
        tft->print("C");
        tft->setTextSize(1);
        tft->setCursor(cx+4, cy+2);
        tft->print("F");
        break;
      case 18: // Dés
        tft->fillCircle(cx, cy, 11, TFT_WHITE);
        tft->fillCircle(cx-4, cy-4, 2, TFT_BLACK);
        tft->fillCircle(cx+4, cy-4, 2, TFT_BLACK);
        tft->fillCircle(cx, cy, 2, TFT_BLACK);
        tft->fillCircle(cx-4, cy+4, 2, TFT_BLACK);
        tft->fillCircle(cx+4, cy+4, 2, TFT_BLACK);
        break;
      case 19: // Livre
        tft->drawRect(cx-11, cy-10, 22, 22, TFT_WHITE);
        tft->drawFastVLine(cx, cy-10, 22, TFT_WHITE);
        tft->drawFastHLine(cx-8, cy-5, 6, TFT_WHITE);
        tft->drawFastHLine(cx-8, cy, 6, TFT_WHITE);
        tft->drawFastHLine(cx-8, cy+5, 6, TFT_WHITE);
        tft->drawFastHLine(cx+2, cy-5, 6, TFT_WHITE);
        tft->drawFastHLine(cx+2, cy, 6, TFT_WHITE);
        tft->drawFastHLine(cx+2, cy+5, 6, TFT_WHITE);
        break;
    }
  }

  void drawIcon(int idx, int x, int y, int size) {
    tft->fillRoundRect(x + 2, y + 2, size, size, 12, 0x0000);
    tft->fillRoundRect(x, y, size, size, 12, apps[idx]->iconColor);
    tft->drawRoundRect(x, y, size, size, 12, TFT_WHITE);
    drawIconSymbol(x, y, idx, size);

    tft->setTextColor(TFT_WHITE, tft->color565(20, 25, 50));
    tft->setTextSize(1);
    int nameLen = apps[idx]->name.length() * 6;
    tft->setCursor(x + (size - nameLen) / 2, y + size + 2);
    tft->print(apps[idx]->name);
  }

public:
  Launcher(TFT_eSPI* display) : tft(display) {
    apps[0]  = createCalculator();
    apps[1]  = createSettings();
    apps[2]  = createClock();
    apps[3]  = createNotes();
    apps[4]  = createWiFi();
    apps[5]  = createMusic();
    apps[6]  = createGallery();
    apps[7]  = createWeather();
    apps[8]  = createGame();
    apps[9]  = createFiles();
    apps[10] = createCalendar();     // ⭐ nouvelle
    apps[11] = createAlarm();        // ⭐
    apps[12] = createCamera();       // ⭐
    apps[13] = createPaint();        // ⭐
    apps[14] = createPong();         // ⭐
    apps[15] = createTicTacToe();    // ⭐
    apps[16] = createSysInfo();      // ⭐
    apps[17] = createConverter();    // ⭐
    apps[18] = createDice();         // ⭐
    apps[19] = createBook();         // ⭐
  }

  // ============================================================
  // DESSIN DU HOME (adapté pour nav bar)
  // ============================================================
  void draw() {
    drawBackground();
    drawTimeWidget();

    // Grille 2x3
    int startX = 22;
    int startY = 88;
    int iconSize = 58;
    int spacingX = 32;
    int spacingY = 2;

    int startIdx = currentPage * iconsPerPage;
    for (int i = 0; i < iconsPerPage; i++) {
      int idx = startIdx + i;
      if (idx >= appCount) break;
      int col = i % 2;
      int row = i / 2;
      int x = startX + col * (iconSize + spacingX);
      int y = startY + row * (iconSize + spacingY);
      drawIcon(idx, x, y, iconSize);
    }

    // Flèches de navigation
    int totalPages = (appCount + iconsPerPage - 1) / iconsPerPage;

    if (currentPage > 0) {
      tft->fillRoundRect(15, 240, 40, 22, 6, tft->color565(0, 150, 200));
      tft->drawRoundRect(15, 240, 40, 22, 6, TFT_WHITE);
      tft->fillTriangle(42, 251, 30, 244, 30, 258, TFT_WHITE);
    } else {
      tft->drawRoundRect(15, 240, 40, 22, 6, tft->color565(80, 80, 100));
      tft->fillTriangle(42, 251, 30, 244, 30, 258, tft->color565(80, 80, 100));
    }

    if (currentPage < totalPages - 1) {
      tft->fillRoundRect(185, 240, 40, 22, 6, tft->color565(0, 150, 200));
      tft->drawRoundRect(185, 240, 40, 22, 6, TFT_WHITE);
      tft->fillTriangle(198, 251, 210, 244, 210, 258, TFT_WHITE);
    } else {
      tft->drawRoundRect(185, 240, 40, 22, 6, tft->color565(80, 80, 100));
      tft->fillTriangle(198, 251, 210, 244, 210, 258, tft->color565(80, 80, 100));
    }

    // Indicateur de page
    tft->setTextColor(TFT_WHITE, tft->color565(20, 30, 60));
    tft->setTextSize(1);
    String pageStr = String(currentPage + 1) + "/" + String(totalPages);
    int txtLen = pageStr.length() * 6;
    tft->setCursor(120 - txtLen / 2, 247);
    tft->print(pageStr);

    // Petit dock décoratif (au-dessus de la nav bar)
    tft->fillRoundRect(30, 268, 180, 16, 8, tft->color565(25, 35, 70));
  }

  // ============================================================
  // GESTION DU TOUCHER
  // ============================================================
  App* handleTouch(int x, int y) {
    int startX = 22;
    int startY = 88;
    int iconSize = 58;
    int spacingX = 32;
    int spacingY = 2;
    int totalPages = (appCount + iconsPerPage - 1) / iconsPerPage;

    // Flèche GAUCHE
    if (x >= 15 && x <= 55 && y >= 240 && y <= 262) {
      if (currentPage > 0) {
        currentPage--;
        draw();
      }
      return nullptr;
    }

    // Flèche DROITE
    if (x >= 185 && x <= 225 && y >= 240 && y <= 262) {
      if (currentPage < totalPages - 1) {
        currentPage++;
        draw();
      }
      return nullptr;
    }

    // Icônes
    int startIdx = currentPage * iconsPerPage;
    for (int i = 0; i < iconsPerPage; i++) {
      int idx = startIdx + i;
      if (idx >= appCount) break;
      int col = i % 2;
      int row = i / 2;
      int ix = startX + col * (iconSize + spacingX);
      int iy = startY + row * (iconSize + spacingY);

      if (x >= ix && x <= ix + iconSize && y >= iy && y <= iy + iconSize) {
        return apps[idx];
      }
    }
    return nullptr;
  }
};

#endif