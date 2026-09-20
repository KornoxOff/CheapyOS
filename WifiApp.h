#ifndef WIFIAPP_H
#define WIFIAPP_H

#include "App.h"
#include <WiFi.h>

class WiFiApp : public App {
private:
  int networks = 0;
  String ssids[15];
  int rssis[15];
  bool encrypted[15];
  bool scanning = false;
  int scroll = 0;
  unsigned long lastScan = 0;

  const int listStartY = 100;
  const int listItemH = 34;
  const int maxVisible = 5;

  uint16_t getSignalColor(int rssi) {
    if (rssi > -60) return 0x07E0;   // vert
    if (rssi > -75) return 0xFFE0;   // jaune
    if (rssi > -85) return 0xFD20;   // orange
    return 0xF800;                    // rouge
  }

  int getSignalStrength(int rssi) {
    if (rssi > -60) return 3;
    if (rssi > -75) return 2;
    if (rssi > -85) return 1;
    return 0;
  }

  void drawWifiIcon(int x, int y, int strength, uint16_t color) {
    // 3 arcs + point
    for (int i = 0; i < strength; i++) {
      int r = 4 + i * 4;
      tft->drawCircleHelper(x + 8, y + 12, r, 0x0C, color);
    }
    tft->fillCircle(x + 8, y + 12, 2, color);
  }

  void drawHeader() {
    // Fond header
    tft->fillRect(0, 24, 240, 70, tft->color565(20, 30, 60));

    tft->setTextColor(TFT_WHITE, tft->color565(20, 30, 60));
    tft->setTextSize(2);
    tft->setCursor(15, 35);
    tft->print("WiFi");

    tft->setTextSize(1);
    tft->setTextColor(tft->color565(180, 200, 255), tft->color565(20, 30, 60));
    tft->setCursor(15, 60);
    tft->print(String(networks) + " reseaux detectes");

    // Bouton Scanner (grand et bien placé)
    tft->fillRoundRect(15, 78, 210, 16, 6, tft->color565(0, 150, 200));
    tft->setTextColor(TFT_WHITE, tft->color565(0, 150, 200));
    tft->setTextSize(1);
    tft->setCursor(90, 82);
    tft->print("SCANNER");
  }

  void drawList() {
    // Fond de la liste
    tft->fillRect(0, listStartY, 240, 200, 0x0841);

    if (scanning) {
      tft->setTextColor(TFT_YELLOW, 0x0841);
      tft->setTextSize(2);
      tft->setCursor(40, 180);
      tft->print("Recherche...");
      return;
    }

    if (networks == 0) {
      tft->setTextColor(tft->color565(150, 150, 150), 0x0841);
      tft->setTextSize(1);
      tft->setCursor(50, 180);
      tft->print("Aucun reseau detecte");
      return;
    }

    for (int i = scroll; i < networks && i < scroll + maxVisible; i++) {
      int y = listStartY + (i - scroll) * listItemH;

      // Fond alterné
      tft->fillRoundRect(8, y + 2, 224, listItemH - 4, 6,
                        (i % 2 == 0) ? tft->color565(25, 35, 65) : tft->color565(20, 28, 55));

      // Icône WiFi
      drawWifiIcon(15, y + 4, getSignalStrength(rssis[i]), getSignalColor(rssis[i]));

      // SSID
      tft->setTextColor(TFT_WHITE, (i % 2 == 0) ? tft->color565(25, 35, 65) : tft->color565(20, 28, 55));
      tft->setTextSize(1);
      tft->setCursor(45, y + 8);
      String ssid = ssids[i];
      if (ssid.length() > 20) ssid = ssid.substring(0, 18) + "..";
      tft->print(ssid);

      // RSSI
      tft->setCursor(45, y + 20);
      tft->setTextColor(getSignalColor(rssis[i]),
                       (i % 2 == 0) ? tft->color565(25, 35, 65) : tft->color565(20, 28, 55));
      tft->print(String(rssis[i]) + " dBm");

      // Cadenas si sécurisé
      if (encrypted[i]) {
        tft->drawRect(210, y + 12, 8, 7, TFT_YELLOW);
        tft->drawCircleHelper(214, y + 8, 4, 0x03, TFT_YELLOW);
        tft->drawCircleHelper(214, y + 8, 4, 0x0C, TFT_YELLOW);
      }
    }

    // Indicateur de scroll
    if (networks > maxVisible) {
      int totalH = networks * listItemH;
      int barH = max(20, 200 * maxVisible / networks);
      int barY = listStartY + (200 - barH) * scroll / max(1, networks - maxVisible);
      tft->fillRoundRect(232, barY, 4, barH, 2, tft->color565(100, 150, 200));
    }
  }

public:
  WiFiApp() : App("WiFi", 0x07E0) {}

  void draw() override {
    tft->fillScreen(0x0841);
    drawHeader();
    drawList();
  }

  void handleTouch(int x, int y) override {
    // Bouton Scanner
    if (y >= 78 && y <= 94 && x >= 15 && x <= 225) {
      scanning = true;
      draw();

      WiFi.mode(WIFI_STA);
      WiFi.disconnect();
      delay(100);

      int n = WiFi.scanNetworks();
      networks = constrain(n, 0, 15);
      for (int i = 0; i < networks; i++) {
        ssids[i] = WiFi.SSID(i);
        rssis[i] = WiFi.RSSI(i);
        encrypted[i] = (WiFi.encryptionType(i) != WIFI_AUTH_OPEN);
      }

      // Trier par RSSI (du plus fort au plus faible)
      for (int i = 0; i < networks - 1; i++) {
        for (int j = i + 1; j < networks; j++) {
          if (rssis[i] < rssis[j]) {
            int tR = rssis[i]; rssis[i] = rssis[j]; rssis[j] = tR;
            String tS = ssids[i]; ssids[i] = ssids[j]; ssids[j] = tS;
            bool tE = encrypted[i]; encrypted[i] = encrypted[j]; encrypted[j] = tE;
          }
        }
      }

      scanning = false;
      lastScan = millis();
      draw();
      return;
    }

    // Scroll
    if (y > listStartY) {
      if (y < 120 && scroll > 0) { scroll--; draw(); }
      else if (y > 260 && scroll < networks - maxVisible) { scroll++; draw(); }
    }
  }
};

App* createWiFi() { return new WiFiApp(); }

#endif