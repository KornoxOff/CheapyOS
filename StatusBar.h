#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <TFT_eSPI.h>
#include <WiFi.h>
#include <ESP32Time.h>

// Couleurs de la barre
#define SB_BG_COLOR      0x2104    // Gris foncé
#define SB_TEXT_COLOR    TFT_WHITE
#define SB_HEIGHT        22
#define SB_SCREEN_WIDTH  320

class StatusBar {
private:
  TFT_eSPI* tft;
  ESP32Time* rtc;
  
  int batteryLevel = 100;
  bool charging = false;
  unsigned long lastUpdate = 0;
  unsigned long lastMillis = 0;
  
  // Notifications
  String notificationText = "";
  unsigned long notificationEnd = 0;
  
  // État
  bool wifiConnected = false;
  int wifiRSSI = 0;
  bool bluetoothOn = false;
  bool silentMode = false;
  
public:
  StatusBar(TFT_eSPI* display) : tft(display), rtc(nullptr) {}
  
  void setRTC(ESP32Time* r) { rtc = r; }
  
  // ================================
  // DESSIN PRINCIPAL
  // ================================
  void draw() {
    // Fond
    tft->fillRect(0, 0, SB_SCREEN_WIDTH, SB_HEIGHT, SB_BG_COLOR);
    tft->drawFastHLine(0, SB_HEIGHT - 1, SB_SCREEN_WIDTH, 0x4208);
    
    // Sections
    drawLogo();
    drawWifiIcon();
    drawBatteryIcon();
    drawClock();
    drawNotifications();
    
    if (bluetoothOn) drawBluetoothIcon();
    if (silentMode) drawSilentIcon();
  }
  
  // ================================
  // LOGO / NOM SYSTÈME
  // ================================
  void drawLogo() {
    tft->setTextColor(SB_TEXT_COLOR, SB_BG_COLOR);
    tft->setTextSize(1);
    tft->setCursor(4, 7);
    tft->print("ESP32-OS");
  }
  
  // ================================
  // ICÔNE WIFI
  // ================================
  void drawWifiIcon() {
    int x = 190, y = 5;
    
    // Effacer la zone
    tft->fillRect(x - 2, 0, 22, SB_HEIGHT, SB_BG_COLOR);
    
    if (WiFi.status() == WL_CONNECTED) {
      wifiConnected = true;
      wifiRSSI = WiFi.RSSI();
      
      // Puissance du signal (nombre d'arcs)
      int strength = 1;
      if (wifiRSSI > -60) strength = 3;
      else if (wifiRSSI > -75) strength = 2;
      
      uint16_t color = (strength == 3) ? TFT_GREEN : 
                       (strength == 2) ? TFT_YELLOW : TFT_ORANGE;
      
      // Dessiner les arcs WiFi
      for (int i = 0; i < strength; i++) {
        int r = 3 + i * 3;
        tft->drawCircleHelper(x + 8, y + 14, r, 0x0C, color);
      }
      tft->fillCircle(x + 8, y + 14, 1, color);
      
    } else {
      wifiConnected = false;
      
      // Icône WiFi barrée
      tft->drawCircleHelper(x + 8, y + 14, 3, 0x0C, TFT_DARKGREY);
      tft->drawCircleHelper(x + 8, y + 14, 6, 0x0C, TFT_DARKGREY);
      tft->drawLine(x + 2, y + 3, x + 14, y + 15, TFT_RED);
      tft->drawLine(x + 3, y + 3, x + 15, y + 15, TFT_RED);
    }
  }
  
  // ================================
  // ICÔNE BATTERIE
  // ================================
  void drawBatteryIcon() {
    int x = 275, y = 5;
    int w = 30, h = 13;
    
    // Corps de la batterie
    tft->drawRect(x, y, w, h, TFT_WHITE);
    tft->fillRect(x + w, y + 4, 3, 5, TFT_WHITE); // Borne +
    
    // Niveau
    int fillW = (w - 4) * batteryLevel / 100;
    uint16_t color = (batteryLevel > 50) ? TFT_GREEN :
                     (batteryLevel > 20) ? TFT_YELLOW : TFT_RED;
    
    if (batteryLevel > 0) {
      tft->fillRect(x + 2, y + 2, fillW, h - 4, color);
    }
    
    // Éclair si en charge
    if (charging) {
      tft->fillTriangle(x + 12, y + 2,
                        x + 18, y + 6,
                        x + 14, y + 6, TFT_WHITE);
      tft->fillTriangle(x + 14, y + 6,
                        x + 18, y + 6,
                        x + 14, y + 11, TFT_WHITE);
    }
  }
  
  // ================================
  // HORLOGE
  // ================================
  void drawClock() {
    if (!rtc) return;
    
    // Effacer la zone
    tft->fillRect(215, 0, 60, SB_HEIGHT, SB_BG_COLOR);
    
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", rtc->getHour(true), rtc->getMinute());
    
    tft->setTextColor(SB_TEXT_COLOR, SB_BG_COLOR);
    tft->setTextSize(1);
    tft->setCursor(222, 7);
    tft->print(timeStr);
    
    // Indicateur AM/PM
    tft->setTextSize(1);
    tft->setCursor(258, 4);
    tft->print(rtc->getHour(true) < 12 ? "AM" : "PM");
  }
  
  // ================================
  // ICÔNE BLUETOOTH
  // ================================
  void drawBluetoothIcon() {
    int x = 175, y = 6;
    tft->setTextColor(0x001F, SB_BG_COLOR);
    tft->setTextSize(1);
    tft->setCursor(x, y);
    tft->print("B");
  }
  
  // ================================
  // ICÔNE MODE SILENCIEUX
  // ================================
  void drawSilentIcon() {
    int x = 165, y = 6;
    tft->setTextColor(TFT_YELLOW, SB_BG_COLOR);
    tft->setTextSize(1);
    tft->setCursor(x, y);
    tft->print("M");
  }
  
  // ================================
  // NOTIFICATIONS
  // ================================
  void drawNotifications() {
    if (notificationText.length() == 0) return;
    if (millis() > notificationEnd) {
      notificationText = "";
      return;
    }
    
    // Afficher la notification au centre
    tft->fillRoundRect(80, 0, 90, SB_HEIGHT, 4, 0x001F);
    tft->setTextColor(TFT_WHITE, 0x001F);
    tft->setTextSize(1);
    
    int textW = notificationText.length() * 6;
    tft->setCursor(80 + (90 - textW) / 2, 8);
    tft->print(notificationText);
  }
  
  void showNotification(String msg, int durationMs = 3000) {
    notificationText = msg;
    notificationEnd = millis() + durationMs;
    draw();
  }
  
  // ================================
  // MISE À JOUR DYNAMIQUE
  // ================================
  void update() {
    unsigned long now = millis();
    
    // Mise à jour toutes les secondes
    if (now - lastUpdate > 1000) {
      lastUpdate = now;
      drawClock();
      drawWifiIcon();
      drawNotifications();
    }
    
    // Simulation de la batterie (décharge lente)
    if (now - lastMillis > 60000) {
      lastMillis = now;
      if (batteryLevel > 0 && !charging) {
        batteryLevel--;
        drawBatteryIcon();
      }
    }
  }
  
  // ================================
  // SETTERS / API PUBLIQUE
  // ================================
  void setBattery(int level, bool isCharging = false) {
    batteryLevel = constrain(level, 0, 100);
    charging = isCharging;
    drawBatteryIcon();
  }
  
  void setBluetooth(bool on) {
    bluetoothOn = on;
    draw();
  }
  
  void setSilentMode(bool on) {
    silentMode = on;
    draw();
  }
  
  bool isTouched(int x, int y) {
    return (y < SB_HEIGHT);
  }
  
  // Actions au toucher de la barre
  void handleTouch(int x, int y) {
    if (y >= SB_HEIGHT) return;
    
    // Zone horloge → ouvrir l'app Horloge
    if (x > 215 && x < 275) {
      showNotification("Horloge");
      // Le System peut ouvrir ClockApp ici
    }
    // Zone batterie → ouvrir paramètres batterie
    else if (x > 275) {
      showNotification("Batterie " + String(batteryLevel) + "%");
    }
    // Zone WiFi → ouvrir paramètres WiFi
    else if (x > 185 && x < 215) {
      if (wifiConnected) {
        showNotification("WiFi OK " + String(wifiRSSI) + "dBm");
      } else {
        showNotification("WiFi déconnecté");
      }
    }
    // Logo → retour accueil
    else if (x < 60) {
      showNotification("Accueil");
    }
  }
  
  int getHeight() { return SB_HEIGHT; }
  int getBatteryLevel() { return batteryLevel; }
};

#endif