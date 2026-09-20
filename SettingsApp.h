#ifndef SETTINGSAPP_H
#define SETTINGSAPP_H

#include "App.h"
#include <WiFi.h>

class SettingsApp : public App {
private:
  int menuLevel = 0;  // 0=principal, 1=WiFi, 2=BT, 3=Système, 4=Affichage, 5=Batterie, 6=Stockage, 7=À propos
  int scroll = 0;
  int btScan = 0;

  // WiFi
  int wifiCount = 0;
  String wifiSSIDs[10];
  int wifiRSSIs[10];
  bool wifiEnc[10];
  bool wifiScanning = false;

  // Structure menu
  struct MenuItem {
    String label;
    String subtitle;
    uint16_t iconColor;
    int iconId;
    int nextLevel;
  };

  // ================= MENU PRINCIPAL =================
  MenuItem mainMenu[10] = {
    {"Reseau et Internet",  "", 0x07E0, 0, 1},
    {"Appareils connectes", "", 0x001F, 1, 2},
    {"Applications",        "", 0xFD20, 6, 0},
    {"Notifications",       "", 0xFFE0, 2, 0},
    {"Batterie",            "", 0x07FF, 3, 5},
    {"Stockage",            "", 0x7BEF, 7, 6},
    {"Affichage",           "", 0xF81F, 4, 4},
    {"Son",                 "", 0xFFE0, 8, 0},
    {"Systeme",             "", 0x7BEF, 5, 3},
    {"A propos",            "", 0x07FF, 9, 7}
  };

  // ============================================================
  // ICÔNES DESSINÉES
  // ============================================================
  void drawIcon(int x, int y, int iconId, uint16_t color) {
    int cx = x + 14;
    int cy = y + 14;

    switch(iconId) {
      case 0: // WiFi
        for (int i = 1; i <= 3; i++)
          tft->drawCircleHelper(cx, cy + 6, i * 4, 0x0C, color);
        tft->fillCircle(cx, cy + 6, 2, color);
        break;
      case 1: // Bluetooth
        tft->drawLine(cx, cy - 10, cx, cy + 10, color);
        tft->drawLine(cx, cy - 10, cx + 6, cy - 4, color);
        tft->drawLine(cx + 6, cy - 4, cx - 6, cy + 4, color);
        tft->drawLine(cx, cy + 10, cx + 6, cy + 4, color);
        tft->drawLine(cx + 6, cy + 4, cx - 6, cy - 4, color);
        break;
      case 2: // Notifications
        tft->drawCircle(cx, cy, 10, color);
        tft->fillTriangle(cx - 6, cy + 6, cx + 6, cy + 6, cx, cy + 12, color);
        tft->fillRect(cx - 6, cy - 6, 12, 12, color);
        break;
      case 3: // Batterie
        tft->drawRect(cx - 10, cy - 5, 18, 10, color);
        tft->fillRect(cx + 8, cy - 2, 3, 4, color);
        tft->fillRect(cx - 8, cy - 3, 12, 6, color);
        break;
      case 4: // Affichage
        tft->fillCircle(cx, cy, 6, color);
        for (int i = 0; i < 8; i++) {
          float a = i * PI / 4;
          tft->drawLine(cx + cos(a)*9, cy + sin(a)*9,
                        cx + cos(a)*13, cy + sin(a)*13, color);
        }
        break;
      case 5: // Système
        tft->drawRect(cx - 10, cy - 8, 20, 14, color);
        tft->drawFastHLine(cx - 4, cy + 9, 8, color);
        tft->fillRect(cx - 7, cy - 5, 14, 2, color);
        tft->fillRect(cx - 7, cy - 1, 10, 2, color);
        break;
      case 6: // Applications
        for (int i = 0; i < 3; i++)
          for (int j = 0; j < 3; j++)
            tft->fillRect(cx - 8 + i * 6, cy - 8 + j * 6, 4, 4, color);
        break;
      case 7: // Stockage
        tft->drawRect(cx - 10, cy - 6, 20, 12, color);
        tft->drawFastHLine(cx - 10, cy, 20, color);
        tft->fillRect(cx - 7, cy - 3, 4, 6, color);
        break;
      case 8: // Son
        tft->fillRect(cx - 8, cy - 4, 5, 8, color);
        tft->fillTriangle(cx - 8, cy - 4, cx - 8, cy + 4, cx - 13, cy + 8, color);
        tft->fillTriangle(cx - 8, cy - 4, cx - 8, cy + 4, cx - 13, cy - 8, color);
        break;
      case 9: // Info
        tft->drawCircle(cx, cy, 11, color);
        tft->fillCircle(cx, cy - 5, 2, color);
        tft->fillRect(cx - 2, cy - 1, 4, 8, color);
        break;
    }
  }

  // ============================================================
  // BARRE HAUTE avec titre et bouton retour
  // ============================================================
  void drawHeader(String title, bool showBack) {
    tft->fillRect(0, 26, 240, 32, tft->color565(20, 25, 50));

    if (showBack) {
      // Bouton retour
      tft->fillCircle(20, 42, 12, tft->color565(40, 50, 80));
      tft->fillTriangle(24, 42, 16, 36, 16, 48, TFT_WHITE);
    }

    tft->setTextColor(TFT_WHITE, tft->color565(20, 25, 50));
    tft->setTextSize(2);
    int x = showBack ? 45 : 12;
    tft->setCursor(x, 35);
    tft->print(title);
  }

  // ============================================================
  // MENU PRINCIPAL
  // ============================================================
  void drawMainMenu() {
    // Fond
    for (int y = 26; y < 320; y++) {
      uint8_t v = map(y, 26, 320, 15, 25);
      tft->drawFastHLine(0, y, 240, tft->color565(v, v, v + 10));
    }

    drawHeader("Parametres", false);

    int y0 = 70;
    int itemH = 45;

    for (int i = 0; i < 10; i++) {
      int y = y0 + i * itemH;
      if (y > 320) break;

      // Fond item
      tft->fillRoundRect(8, y, 224, 40, 8, tft->color565(35, 35, 42));

      // Icône ronde
      tft->fillCircle(30, y + 20, 16, tft->color565(50, 50, 60));
      drawIcon(16, y + 6, mainMenu[i].iconId, mainMenu[i].iconColor);

      // Label
      tft->setTextColor(TFT_WHITE, tft->color565(35, 35, 42));
      tft->setTextSize(1);
      tft->setCursor(55, y + 14);
      tft->print(mainMenu[i].label);

      // Chevron >
      tft->setTextColor(tft->color565(150, 150, 150), tft->color565(35, 35, 42));
      tft->setCursor(215, y + 17);
      tft->print(">");
    }
  }

  // ============================================================
  // SOUS-MENU WIFI
  // ============================================================
  void drawWiFiMenu() {
    for (int y = 26; y < 320; y++) {
      uint8_t v = map(y, 26, 320, 15, 25);
      tft->drawFastHLine(0, y, 240, tft->color565(v, v, v + 10));
    }

    drawHeader("WiFi", true);

    // Bouton scan
    tft->fillRoundRect(15, 70, 210, 35, 8, wifiScanning ? 0xFD20 : 0x07E0);
    tft->setTextColor(TFT_WHITE, wifiScanning ? 0xFD20 : 0x07E0);
    tft->setTextSize(2);
    tft->setCursor(85, 80);
    tft->print(wifiScanning ? "SCAN..." : "SCANNER");

    // État connexion
    int y = 115;
    if (WiFi.status() == WL_CONNECTED) {
      tft->fillRoundRect(8, y, 224, 40, 8, tft->color565(0, 80, 40));
      tft->setTextColor(TFT_WHITE, tft->color565(0, 80, 40));
      tft->setTextSize(1);
      tft->setCursor(18, y + 8);
      tft->print("Connecte a:");
      tft->setTextSize(2);
      tft->setCursor(18, y + 20);
      tft->print(WiFi.SSID());
      y += 50;
    }

    // Liste WiFi
    for (int i = 0; i < wifiCount && i < 5; i++) {
      int itemY = y + i * 32;
      if (itemY > 290) break;

      tft->fillRoundRect(8, itemY, 224, 28, 6, tft->color565(30, 30, 38));

      // Signal
      int strength = (wifiRSSIs[i] > -60) ? 3 : (wifiRSSIs[i] > -75) ? 2 : 1;
      uint16_t sigColor = (strength == 3) ? 0x07E0 : (strength == 2) ? 0xFFE0 : 0xFD20;
      for (int s = 0; s < strength; s++) {
        tft->drawCircleHelper(20, itemY + 18, 3 + s * 3, 0x0C, sigColor);
      }

      // SSID
      tft->setTextColor(TFT_WHITE, tft->color565(30, 30, 38));
      tft->setTextSize(1);
      tft->setCursor(45, itemY + 10);
      String ssid = wifiSSIDs[i];
      if (ssid.length() > 18) ssid = ssid.substring(0, 16) + "..";
      tft->print(ssid);

      // Cadenas
      if (wifiEnc[i]) {
        tft->drawRect(210, itemY + 10, 8, 8, TFT_YELLOW);
        tft->drawCircleHelper(214, itemY + 6, 4, 0x03, TFT_YELLOW);
        tft->drawCircleHelper(214, itemY + 6, 4, 0x0C, TFT_YELLOW);
      }
    }

    if (wifiCount == 0 && !wifiScanning) {
      tft->setTextColor(tft->color565(150, 150, 150), tft->color565(15, 15, 25));
      tft->setTextSize(1);
      tft->setCursor(70, 200);
      tft->print("Touchez SCANNER");
    }
  }

  // ============================================================
  // SOUS-MENU BLUETOOTH
  // ============================================================
  void drawBluetoothMenu() {
    for (int y = 26; y < 320; y++) {
      uint8_t v = map(y, 26, 320, 15, 25);
      tft->drawFastHLine(0, y, 240, tft->color565(v, v, v + 10));
    }

    drawHeader("Bluetooth", true);

    // Toggle
    tft->fillRoundRect(8, 70, 224, 45, 8, tft->color565(35, 35, 42));
    tft->setTextColor(TFT_WHITE, tft->color565(35, 35, 42));
    tft->setTextSize(1);
    tft->setCursor(55, 82);
    tft->print("Bluetooth");
    tft->setCursor(55, 96);
    tft->setTextColor(tft->color565(150, 150, 150), tft->color565(35, 35, 42));
    tft->print("Active/Desactive");

    // Toggle BT (décoratif)
    tft->fillRoundRect(180, 80, 45, 22, 11, 0x07E0);
    tft->fillCircle(215, 91, 9, TFT_WHITE);

    // Appareils à proximité
    tft->setTextColor(tft->color565(150, 200, 255), tft->color565(15, 15, 25));
    tft->setTextSize(1);
    tft->setCursor(15, 130);
    tft->print("Appareils a proximite");

    // Bouton scan BT
    tft->fillRoundRect(15, 150, 210, 30, 6, 0x001F);
    tft->setTextColor(TFT_WHITE, 0x001F);
    tft->setTextSize(1);
    tft->setCursor(85, 160);
    tft->print("RECHERCHER");

    // Placeholder
    tft->setTextColor(tft->color565(150, 150, 150), tft->color565(15, 15, 25));
    tft->setCursor(40, 220);
    tft->print("Aucun appareil trouve");

    tft->setCursor(40, 240);
    tft->print("(placeholder)");
  }

  // ============================================================
  // SOUS-MENU SYSTÈME
  // ============================================================
  void drawSystemMenu() {
    for (int y = 26; y < 320; y++) {
      uint8_t v = map(y, 26, 320, 15, 25);
      tft->drawFastHLine(0, y, 240, tft->color565(v, v, v + 10));
    }

    drawHeader("Systeme", true);

    int y = 70;
    int itemH = 22;

    auto addItem = [&](String label, String value) {
      tft->setTextColor(tft->color565(150, 180, 220), tft->color565(15, 15, 25));
      tft->setTextSize(1);
      tft->setCursor(15, y);
      tft->print(label);

      tft->setTextColor(TFT_WHITE, tft->color565(15, 15, 25));
      tft->setCursor(120, y);
      tft->print(value);
      y += itemH;
    };

    addItem("Modele:",   ESP.getChipModel());
    addItem("Revision:", String(ESP.getChipRevision()));
    addItem("Coeurs:",   String(ESP.getChipCores()));
    addItem("Frequence:",String(ESP.getCpuFreqMHz()) + " MHz");
    addItem("Flash:",    String(ESP.getFlashChipSize() / 1048576) + " MB");
    addItem("Flash Freq:",String(ESP.getFlashChipSpeed() / 1000000) + " MHz");
    addItem("PSRAM:",    String(ESP.getPsramSize() / 1048576) + " MB");
    addItem("Heap libre:",String(ESP.getFreeHeap() / 1024) + " KB");
    addItem("Heap max:", String(ESP.getMaxAllocHeap() / 1024) + " KB");
    addItem("SDK:",      ESP.getSdkVersion());
    addItem("Uptime:",   String(millis() / 1000) + " s");
    addItem("Chip ID:",  String((uint32_t)ESP.getEfuseMac(), HEX));
    addItem("WiFi:",     WiFi.status() == WL_CONNECTED ? "Connecte" : "Non connecte");
    if (WiFi.status() == WL_CONNECTED) {
      addItem("IP:",     WiFi.localIP().toString());
      addItem("RSSI:",   String(WiFi.RSSI()) + " dBm");
      addItem("SSID:",   WiFi.SSID());
    }
    addItem("MAC WiFi:", WiFi.macAddress());
  }

  // ============================================================
  // SOUS-MENU AFFICHAGE
  // ============================================================
  void drawDisplayMenu() {
    for (int y = 26; y < 320; y++) {
      uint8_t v = map(y, 26, 320, 15, 25);
      tft->drawFastHLine(0, y, 240, tft->color565(v, v, v + 10));
    }

    drawHeader("Affichage", true);

    // Luminosité
    tft->setTextColor(TFT_WHITE, tft->color565(15, 15, 25));
    tft->setTextSize(1);
    tft->setCursor(15, 80);
    tft->print("Luminosite");

    tft->fillRoundRect(15, 100, 210, 10, 5, tft->color565(50, 50, 70));
    tft->fillRoundRect(15, 100, 160, 10, 5, 0x07E0);
    tft->fillCircle(175, 105, 10, TFT_WHITE);

    // Rotation
    tft->setTextColor(TFT_WHITE, tft->color565(15, 15, 25));
    tft->setCursor(15, 150);
    tft->print("Rotation ecran");

    tft->fillRoundRect(15, 170, 100, 35, 8, 0x07E0);
    tft->setTextColor(TFT_WHITE, 0x07E0);
    tft->setTextSize(1);
    tft->setCursor(40, 182);
    tft->print("PORTRAIT");

    tft->fillRoundRect(125, 170, 100, 35, 8, 0x4208);
    tft->setTextColor(TFT_WHITE, 0x4208);
    tft->setCursor(145, 182);
    tft->print("PAYSAGE");
  }

  // ============================================================
  // SOUS-MENU BATTERIE
  // ============================================================
  void drawBatteryMenu() {
    for (int y = 26; y < 320; y++) {
      uint8_t v = map(y, 26, 320, 15, 25);
      tft->drawFastHLine(0, y, 240, tft->color565(v, v, v + 10));
    }

    drawHeader("Batterie", true);

    // Grosse batterie
    tft->drawRoundRect(60, 80, 120, 60, 8, TFT_WHITE);
    tft->fillRect(180, 100, 8, 20, TFT_WHITE);
    tft->fillRoundRect(65, 85, 90, 50, 6, 0x07E0);

    tft->setTextColor(TFT_BLACK, 0x07E0);
    tft->setTextSize(4);
    tft->setCursor(85, 95);
    tft->print("85%");

    // Info
    tft->setTextColor(TFT_WHITE, tft->color565(15, 15, 25));
    tft->setTextSize(1);
    tft->setCursor(15, 170);
    tft->print("Etat:  En charge");
    tft->setCursor(15, 195);
    tft->print("Tension:  4.05 V");
    tft->setCursor(15, 220);
    tft->print("Sante:  Bonne");
    tft->setCursor(15, 245);
    tft->print("Temperature: 32 C");
  }

  // ============================================================
  // SOUS-MENU STOCKAGE
  // ============================================================
  void drawStorageMenu() {
    for (int y = 26; y < 320; y++) {
      uint8_t v = map(y, 26, 320, 15, 25);
      tft->drawFastHLine(0, y, 240, tft->color565(v, v, v + 10));
    }

    drawHeader("Stockage", true);

    // Flash
    tft->setTextColor(TFT_WHITE, tft->color565(15, 15, 25));
    tft->setTextSize(1);
    tft->setCursor(15, 80);
    tft->print("Flash interne");

    tft->fillRoundRect(15, 100, 210, 15, 7, tft->color565(50, 50, 70));
    tft->fillRoundRect(15, 100, 100, 15, 7, 0x07E0);

    tft->setTextColor(tft->color565(180, 200, 255), tft->color565(15, 15, 25));
    tft->setCursor(15, 125);
    tft->print("2.1 GB utilises / 4.0 GB");

    // SPIFFS
    tft->setTextColor(TFT_WHITE, tft->color565(15, 15, 25));
    tft->setCursor(15, 170);
    tft->print("SPIFFS");

    tft->fillRoundRect(15, 190, 210, 15, 7, tft->color565(50, 50, 70));
    tft->fillRoundRect(15, 190, 30, 15, 7, 0xFD20);

    tft->setTextColor(tft->color565(180, 200, 255), tft->color565(15, 15, 25));
    tft->setCursor(15, 215);
    tft->print("150 KB utilises / 1 MB");
  }

  // ============================================================
  // SOUS-MENU À PROPOS
  // ============================================================
  void drawAboutMenu() {
    for (int y = 26; y < 320; y++) {
      uint8_t v = map(y, 26, 320, 15, 25);
      tft->drawFastHLine(0, y, 240, tft->color565(v, v, v + 10));
    }

    drawHeader("A propos", true);

    // Logo
    tft->fillCircle(120, 100, 35, 0x07E0);
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(5);
    tft->setCursor(105, 80);
    tft->print("C");

    tft->setTextColor(TFT_WHITE, tft->color565(15, 15, 25));
    tft->setTextSize(2);
    tft->setCursor(75, 145);
    tft->print("CheapyOS");

    tft->setTextColor(tft->color565(180, 200, 255), tft->color565(15, 15, 25));
    tft->setTextSize(1);
    tft->setCursor(100, 170);
    tft->print("Version 4.0");

    tft->setTextColor(tft->color565(150, 150, 150), tft->color565(15, 15, 25));
    tft->setCursor(60, 200);
    tft->print("Systeme embarque ESP32");

    tft->setCursor(70, 220);
    tft->print("Developpe avec Arduino");

    tft->setCursor(85, 240);
    tft->print("Fait maison");

    // Bouton check update
    tft->fillRoundRect(50, 265, 140, 30, 8, 0x07E0);
    tft->setTextColor(TFT_WHITE, 0x07E0);
    tft->setTextSize(1);
    tft->setCursor(65, 275);
    tft->print("VERIFIER MAJ");
  }

public:
  SettingsApp() : App("Parametres", 0x7BEF) {}

  void draw() override {
    switch(menuLevel) {
      case 0: drawMainMenu(); break;
      case 1: drawWiFiMenu(); break;
      case 2: drawBluetoothMenu(); break;
      case 3: drawSystemMenu(); break;
      case 4: drawDisplayMenu(); break;
      case 5: drawBatteryMenu(); break;
      case 6: drawStorageMenu(); break;
      case 7: drawAboutMenu(); break;
    }
  }

  void handleTouch(int x, int y) override {
    // Bouton retour
    if (y >= 30 && y <= 55 && x >= 8 && x <= 35 && menuLevel != 0) {
      menuLevel = 0;
      draw();
      return;
    }

    if (menuLevel == 0) {
      // Menu principal
      int y0 = 70;
      int itemH = 45;
      for (int i = 0; i < 10; i++) {
        int iy = y0 + i * itemH;
        if (y >= iy && y <= iy + 40) {
          menuLevel = mainMenu[i].nextLevel;
          draw();
          return;
        }
      }
    }
    else if (menuLevel == 1) {
      // WiFi - bouton scan
      if (y >= 70 && y <= 105 && x >= 15 && x <= 225) {
        wifiScanning = true;
        draw();

        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(100);

        int n = WiFi.scanNetworks();
        wifiCount = constrain(n, 0, 10);
        for (int i = 0; i < wifiCount; i++) {
          wifiSSIDs[i] = WiFi.SSID(i);
          wifiRSSIs[i] = WiFi.RSSI(i);
          wifiEnc[i] = (WiFi.encryptionType(i) != WIFI_AUTH_OPEN);
        }

        // Trier par RSSI
        for (int i = 0; i < wifiCount - 1; i++) {
          for (int j = i + 1; j < wifiCount; j++) {
            if (wifiRSSIs[i] < wifiRSSIs[j]) {
              int tR = wifiRSSIs[i]; wifiRSSIs[i] = wifiRSSIs[j]; wifiRSSIs[j] = tR;
              String tS = wifiSSIDs[i]; wifiSSIDs[i] = wifiSSIDs[j]; wifiSSIDs[j] = tS;
              bool tE = wifiEnc[i]; wifiEnc[i] = wifiEnc[j]; wifiEnc[j] = tE;
            }
          }
        }

        wifiScanning = false;
        draw();
      }
    }
    else if (menuLevel == 7) {
      // À propos - bouton MAJ
      if (y >= 265 && y <= 295) {
        tft->fillRoundRect(50, 265, 140, 30, 8, 0x07FF);
        tft->setTextColor(TFT_WHITE, 0x07FF);
        tft->setCursor(75, 275);
        tft->print("A JOUR !");
        delay(1000);
        draw();
      }
    }
  }
};

App* createSettings() { return new SettingsApp(); }

#endif