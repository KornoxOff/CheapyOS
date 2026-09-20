#ifndef SYSTEM_H
#define SYSTEM_H

#include <TFT_eSPI.h>
#include <Preferences.h>
#include <WiFi.h>
#include "Launcher.h"

enum OSState {
  STATE_SPLASH,
  STATE_WELCOME,
  STATE_LOCK,
  STATE_HOME,
  STATE_APP
};

class SystemOS {
private:
  TFT_eSPI* tft;
  Launcher* launcher;
  App* currentApp = nullptr;
  OSState state = STATE_SPLASH;
  Preferences prefs;

  String userName = "Utilisateur";
  bool isFirstBoot = true;
  int themeId = 0;

  // ============================================================
  // FOND DÉGRADÉ
  // ============================================================
  void drawGradient(uint16_t topColor, uint16_t botColor) {
    uint8_t tr = (topColor >> 11) & 0x1F;
    uint8_t tg = (topColor >> 5) & 0x3F;
    uint8_t tb = topColor & 0x1F;
    uint8_t br = (botColor >> 11) & 0x1F;
    uint8_t bg = (botColor >> 5) & 0x3F;
    uint8_t bb = botColor & 0x1F;

    for (int y = 0; y < 320; y++) {
      uint8_t r = map(y, 0, 320, tr, br);
      uint8_t g = map(y, 0, 320, tg, bg);
      uint8_t b = map(y, 0, 320, tb, bb);
      tft->drawFastHLine(0, y, 240, (r << 11) | (g << 5) | b);
    }
  }

  void drawThemedBackground() {
    switch(themeId) {
      case 0: drawGradient(tft->color565(15, 30, 70), tft->color565(5, 10, 25)); break;
      case 1: drawGradient(tft->color565(50, 20, 80), tft->color565(15, 5, 35)); break;
      case 2: drawGradient(tft->color565(10, 60, 40), tft->color565(5, 20, 15)); break;
    }
  }

  // ============================================================
  // BOOT LOGO FIXE (7 secondes)
  // ============================================================
  void drawBootLogo() {
    tft->fillScreen(TFT_BLACK);
    delay(300);

    int cx = 120;
    int cy = 130;

    // Halo externe
    for (int r = 80; r > 55; r -= 2) {
      uint8_t a = map(r, 55, 80, 100, 20);
      tft->drawCircle(cx, cy, r, tft->color565(0, a/8, a/2));
    }

    // Cercle principal cyan
    tft->fillCircle(cx, cy, 55, tft->color565(0, 150, 220));

    // Halo interne lumineux
    for (int r = 55; r > 45; r -= 2) {
      uint8_t a = map(r, 45, 55, 220, 150);
      tft->fillCircle(cx, cy, r, tft->color565(a/2, a, 255));
    }

    // "C" blanc au centre
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(7);
    tft->setCursor(96, 100);
    tft->print("C");

    // Nom sous le logo
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(3);
    tft->setCursor(25, 220);
    tft->print("CheapyOS");

    // Petite ligne décorative
    tft->drawFastHLine(80, 255, 80, tft->color565(80, 120, 180));

    // Version
    tft->setTextColor(tft->color565(120, 180, 240));
    tft->setTextSize(1);
    tft->setCursor(105, 265);
    tft->print("v4.0");

    // ⏱️ LE LOGO RESTE FIXE 7 SECONDES
    delay(7000);
  }

  // ============================================================
  // SPLASH : "CheapyOS v4.0"
  // ============================================================
  void showSplash() {
    drawBootLogo();

    tft->fillScreen(TFT_BLACK);
    delay(100);

    for (int i = 0; i < 30; i++) {
      tft->fillScreen(TFT_BLACK);
      int size = i * 2;
      if (size > 60) size = 60;

      for (int r = size; r > 0; r -= 4) {
        uint8_t c = map(r, 0, size, 100, 255);
        tft->fillCircle(120, 130, r, ((c/8) << 11) | ((c/4) << 5) | (c/8));
      }

      tft->setTextColor(TFT_WHITE);
      tft->setTextSize(map(i, 0, 30, 1, 4));
      tft->setCursor(120 - map(i, 0, 30, 3, 12), 130 - map(i, 0, 30, 4, 16));
      tft->print("C");
      delay(15);
    }

    delay(150);
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(3);
    tft->setCursor(25, 210);
    tft->print("CheapyOS");

    tft->setTextColor(tft->color565(120, 200, 255));
    tft->setTextSize(1);
    tft->setCursor(90, 245);
    tft->print("v4.0");

    delay(300);
    tft->drawRoundRect(40, 270, 160, 6, 3, tft->color565(40, 50, 80));
    for (int i = 0; i < 156; i += 4) {
      tft->fillRoundRect(42, 272, i, 2, 1, tft->color565(0, 200, 255));
      delay(10);
    }
    delay(300);
  }

  // ============================================================
  // WELCOME SCREEN
  // ============================================================
  void showWelcome() {
    showWelcomeStep(1, "Choisissez la langue");

    tft->fillRoundRect(20, 130, 200, 40, 10, tft->color565(0, 150, 200));
    tft->drawRoundRect(20, 130, 200, 40, 10, TFT_WHITE);
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(2);
    tft->setCursor(65, 142);
    tft->print("Francais");

    tft->fillRoundRect(20, 180, 200, 40, 10, tft->color565(50, 50, 70));
    tft->drawRoundRect(20, 180, 200, 40, 10, tft->color565(150, 150, 200));
    tft->setTextColor(TFT_WHITE);
    tft->setCursor(75, 192);
    tft->print("English");

    waitForTouch();

    showWelcomeStep(2, "Verifier le WiFi");

    tft->setTextColor(tft->color565(180, 200, 255));
    tft->setTextSize(1);
    tft->setCursor(20, 125);
    tft->print("SSID: Freebox_LE GAL_");

    if (WiFi.status() == WL_CONNECTED) {
      tft->fillRoundRect(20, 150, 200, 50, 10, tft->color565(0, 150, 80));
      tft->setTextColor(TFT_WHITE);
      tft->setTextSize(2);
      tft->setCursor(45, 160);
      tft->print("CONNECTE !");
      tft->setTextSize(1);
      tft->setCursor(60, 185);
      tft->print(WiFi.localIP().toString());
    } else {
      tft->fillRoundRect(20, 150, 200, 50, 10, tft->color565(180, 80, 0));
      tft->setTextColor(TFT_WHITE);
      tft->setTextSize(2);
      tft->setCursor(45, 165);
      tft->print("Connexion...");
    }

    delay(2000);
    waitForTouch();

    showWelcomeStep(3, "Votre nom");

    tft->fillRoundRect(20, 130, 200, 50, 10, tft->color565(30, 40, 70));
    tft->drawRoundRect(20, 130, 200, 50, 10, tft->color565(100, 150, 200));
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(2);
    tft->setCursor(50, 145);
    tft->print("Utilisateur");

    waitForTouch();

    showWelcomeStep(4, "Choisissez un theme");

    int themeY = 130;
    tft->fillRoundRect(15, themeY, 60, 60, 10, tft->color565(0, 100, 180));
    tft->drawRoundRect(15, themeY, 60, 60, 10, TFT_WHITE);
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(1);
    tft->setCursor(30, 200);
    tft->print("Bleu");

    tft->fillRoundRect(90, themeY, 60, 60, 10, tft->color565(120, 60, 180));
    tft->drawRoundRect(90, themeY, 60, 60, 10, TFT_WHITE);
    tft->setCursor(100, 200);
    tft->print("Violet");

    tft->fillRoundRect(165, themeY, 60, 60, 10, tft->color565(0, 150, 100));
    tft->drawRoundRect(165, themeY, 60, 60, 10, TFT_WHITE);
    tft->setCursor(180, 200);
    tft->print("Vert");

    while (true) {
      uint16_t x, y;
      if (tft->getTouch(&x, &y)) {
        if (y >= themeY && y <= themeY + 60) {
          if (x >= 15 && x <= 75) themeId = 0;
          else if (x >= 90 && x <= 150) themeId = 1;
          else if (x >= 165 && x <= 225) themeId = 2;
          tft->drawRoundRect(15 + themeId * 75, themeY - 2, 64, 64, 12, TFT_CYAN);
          delay(300);
          break;
        }
      }
      delay(30);
    }

    drawThemedBackground();
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(3);
    tft->setCursor(30, 120);
    tft->print("Pret !");
    tft->setTextSize(1);
    tft->setTextColor(tft->color565(180, 200, 255));
    tft->setCursor(50, 170);
    tft->print("Configuration terminee");
    delay(1500);

    prefs.begin("cheapyos", false);
    prefs.putBool("firstBoot", false);
    prefs.putInt("theme", themeId);
    prefs.putString("userName", userName);
    prefs.end();
    isFirstBoot = false;

    for (int i = 0; i < 15; i++) {
      tft->fillRect(0, i * 22, 240, 22, TFT_BLACK);
      tft->fillRect(0, 320 - i * 22, 240, 22, TFT_BLACK);
      delay(20);
    }
  }

  void showWelcomeStep(int step, String title) {
    drawThemedBackground();

    for (int i = 0; i < 4; i++) {
      if (i < step) {
        tft->fillCircle(95 + i * 15, 40, 5, tft->color565(0, 200, 255));
      } else {
        tft->drawCircle(95 + i * 15, 40, 5, tft->color565(100, 120, 160));
      }
    }

    tft->setTextColor(tft->color565(150, 180, 220));
    tft->setTextSize(1);
    tft->setCursor(180, 37);
    tft->print("Etape " + String(step) + "/4");

    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(2);
    int tw = title.length() * 12;
    tft->setCursor((240 - tw) / 2, 70);
    tft->print(title);

    tft->fillCircle(120, 105, 12, tft->color565(0, 150, 220));
  }

  void waitForTouch() {
    while (tft->getTouch(NULL, NULL)) delay(30);
    delay(200);
    while (true) {
      uint16_t x, y;
      if (tft->getTouch(&x, &y)) {
        delay(200);
        while (tft->getTouch(&x, &y)) delay(20);
        return;
      }
      delay(20);
    }
  }

  // ============================================================
  // LOCK SCREEN
  // ============================================================
  void showLockScreen() {
    drawThemedBackground();

    tft->setTextColor(TFT_WHITE, tft->color565(15, 30, 70));
    tft->setTextSize(7);
    tft->setCursor(20, 30);
    char timeStr[6];
    unsigned long sec = millis() / 1000;
    sprintf(timeStr, "%02d:%02d", (sec / 60) % 24, sec % 60);
    tft->print(timeStr);

    tft->setTextSize(1);
    tft->setTextColor(tft->color565(180, 200, 255), tft->color565(15, 30, 70));
    tft->setCursor(50, 100);
    tft->print("CheapyOS v4.0");

    int cx = 120;
    int cy = 200;

    tft->drawCircle(cx, cy, 48, tft->color565(80, 100, 150));
    tft->drawCircle(cx, cy, 45, tft->color565(100, 120, 170));
    tft->fillCircle(cx, cy, 40, tft->color565(0, 100, 180));

    tft->fillRoundRect(cx - 18, cy - 3, 36, 28, 4, TFT_WHITE);
    tft->drawRoundRect(cx - 18, cy - 3, 36, 28, 4, tft->color565(200, 200, 220));

    for (int i = 0; i < 3; i++) {
      tft->drawCircleHelper(cx, cy - 8, 13 + i, 0x03, TFT_WHITE);
      tft->drawCircleHelper(cx, cy - 8, 13 + i, 0x0C, TFT_WHITE);
    }

    tft->fillCircle(cx, cy + 8, 4, tft->color565(0, 100, 180));
    tft->fillTriangle(cx - 3, cy + 11, cx + 3, cy + 11, cx, cy + 18, tft->color565(0, 100, 180));

    tft->setTextColor(TFT_WHITE, tft->color565(10, 20, 50));
    tft->setTextSize(1);
    tft->setCursor(65, 268);
    tft->print("Touchez le cadenas");

    uint16_t touchX, touchY;
    bool unlocked = false;

    while (!unlocked) {
      if (tft->getTouch(&touchX, &touchY)) {
        int dx = touchX - cx;
        int dy = touchY - cy;
        int dist = sqrt(dx*dx + dy*dy);

        if (dist < 50) {
          unlocked = true;
          for (int i = 0; i < 4; i++) {
            tft->fillCircle(cx, cy, 40 - i*8, tft->color565(0, 200, 100));
            delay(60);
          }
          tft->fillCircle(cx, cy, 42, tft->color565(0, 200, 100));
          tft->fillRoundRect(cx - 18, cy - 3, 36, 28, 4, TFT_WHITE);
          for (int i = 0; i < 3; i++) {
            tft->drawCircleHelper(cx + 10, cy - 8, 13 + i, 0x03, TFT_WHITE);
          }
          tft->fillCircle(cx, cy + 8, 4, tft->color565(0, 200, 100));
          delay(400);

          for (int r = 10; r < 120; r += 10) {
            tft->drawCircle(cx, cy, r, tft->color565(0, 200, 100));
            delay(20);
          }
          delay(200);
        }
      }
      delay(20);
    }

    for (int i = 0; i < 20; i++) {
      uint8_t v = 255 - i * 13;
      tft->fillScreen(tft->color565(v/6, v/4, v/2));
      delay(15);
    }
  }

  // ============================================================
  // BARRE DE STATUT
  // ============================================================
  void drawStatusBar() {
    for (int y = 0; y < 26; y++) {
      tft->drawFastHLine(0, y, 240, tft->color565(8, 12, 25));
    }
    tft->drawFastHLine(0, 25, 240, tft->color565(60, 80, 130));

    tft->setTextColor(TFT_WHITE, tft->color565(8, 12, 25));
    tft->setTextSize(1);
    unsigned long sec = millis() / 1000;
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", (sec / 60) % 24, sec % 60);
    tft->setCursor(5, 9);
    tft->print(timeStr);

    if (currentApp != nullptr) {
      tft->setTextColor(tft->color565(150, 200, 255), tft->color565(8, 12, 25));
      int nameLen = currentApp->name.length() * 6;
      tft->setCursor(120 - nameLen / 2, 9);
      tft->print(currentApp->name);
    } else {
      tft->setTextColor(TFT_CYAN, tft->color565(8, 12, 25));
      tft->setCursor(105, 9);
      tft->print("CheapyOS");
    }

    tft->setTextColor(TFT_WHITE, tft->color565(8, 12, 25));
    tft->setCursor(180, 9);
    if (WiFi.status() == WL_CONNECTED) {
      tft->print("WiFi");
    } else {
      tft->print("--");
    }

    tft->drawRect(210, 9, 22, 10, TFT_WHITE);
    tft->fillRect(212, 11, 16, 6, TFT_GREEN);
    tft->fillRect(232, 11, 2, 6, TFT_WHITE);
  }

  // ============================================================
  // BARRE DE NAVIGATION
  // ============================================================
  void drawNavBar() {
    for (int y = 290; y < 320; y++) {
      tft->drawFastHLine(0, y, 240, tft->color565(8, 12, 25));
    }
    tft->drawFastHLine(0, 290, 240, tft->color565(60, 80, 130));

    int centerY = 305;

    tft->drawRect(40, centerY - 8, 16, 16, TFT_WHITE);
    tft->fillRect(43, centerY - 5, 10, 10, TFT_WHITE);

    tft->fillCircle(120, centerY, 12, tft->color565(0, 150, 220));
    tft->drawCircle(120, centerY, 12, TFT_WHITE);
    tft->fillRect(116, centerY - 5, 8, 10, TFT_WHITE);

    tft->fillTriangle(180, centerY - 8, 180, centerY + 8, 195, centerY, TFT_WHITE);
  }

  bool handleNavBar(int x, int y) {
    if (y < 290) return false;

    if (x >= 30 && x <= 70) {
      tft->fillRoundRect(20, 200, 200, 80, 12, tft->color565(30, 40, 70));
      tft->drawRoundRect(20, 200, 200, 80, 12, tft->color565(100, 150, 200));
      tft->setTextColor(TFT_WHITE, tft->color565(30, 40, 70));
      tft->setTextSize(1);
      tft->setCursor(35, 215);
      tft->print("Aucune app recente");
      tft->setCursor(35, 240);
      tft->print("(bientot)");
      delay(1500);
      launcher->draw();
      drawStatusBar();
      drawNavBar();
      return true;
    }

    if (x >= 100 && x <= 140) {
      if (currentApp != nullptr) {
        closeApp();
      } else {
        launcher->draw();
        drawStatusBar();
        drawNavBar();
      }
      return true;
    }

    if (x >= 170 && x <= 210) {
      if (currentApp != nullptr) {
        closeApp();
      }
      return true;
    }

    return true;
  }

public:
  SystemOS(TFT_eSPI* display) : tft(display) {}

  void begin() {
    uint16_t calData[5] = { 320, 3500, 350, 3500, 3 };
    tft->setTouch(calData);

    prefs.begin("cheapyos", true);
    isFirstBoot = prefs.getBool("firstBoot", true);
    userName = prefs.getString("userName", "Utilisateur");
    themeId = prefs.getInt("theme", 0);
    prefs.end();

    launcher = new Launcher(tft);
  }

  void boot() {
    showSplash();

    if (isFirstBoot) {
      showWelcome();
    }

    showLockScreen();

    state = STATE_HOME;
    tft->fillScreen(TFT_BLACK);
    launcher->draw();
    drawStatusBar();
    drawNavBar();
  }

  void openApp(App* app) {
    currentApp = app;
    state = STATE_APP;

    for (int s = 5; s < 500; s += 20) {
      tft->fillCircle(120, 145, s, app->iconColor);
      delay(8);
    }

    currentApp->setup(tft);
    currentApp->draw();
    drawStatusBar();
    drawNavBar();
  }

  void closeApp() {
    if (currentApp) {
      for (int i = 0; i < 15; i++) {
        tft->fillRect(0, 290 - i * 20, 240, 20, TFT_BLACK);
        delay(10);
      }
      currentApp->exit();
      currentApp = nullptr;
    }
    state = STATE_HOME;
    launcher->draw();
    drawStatusBar();
    drawNavBar();
  }

  void update() {
    uint16_t x = 0, y = 0;
    bool touching = tft->getTouch(&x, &y);

    if (touching) {
      if (y >= 290) {
        if (handleNavBar(x, y)) {
          delay(200);
          return;
        }
      }

      if (y < 26 && x < 55 && currentApp != nullptr) {
        closeApp();
        delay(200);
        return;
      }

      if (currentApp) {
        currentApp->handleTouch(x, y);
        delay(120);
      } else if (state == STATE_HOME) {
        App* selected = launcher->handleTouch(x, y);
        if (selected) {
          openApp(selected);
        }
        delay(120);
      }
    }

    if (currentApp) currentApp->update();
  }
};

#endif