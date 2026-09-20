#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class WeatherApp : public App {
private:
  float temp = 0;
  String city = "Paris";
  String condition = "Ensoleillé";
  int humidity = 65;
  bool loaded = false;
  
public:
  WeatherApp() : App("Météo", 0x07FF) {}
  
  void draw() override {
    tft->fillScreen(0x0000);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setTextSize(2);
    tft->setCursor(10, 25);
    tft->print("Météo");
    
    // Ville
    tft->setTextSize(2);
    tft->setCursor(110, 55);
    tft->print(city);
    
    // Température
    tft->setTextColor(0xFFE0, TFT_BLACK);
    tft->setTextSize(6);
    tft->setCursor(100, 85);
    tft->print(String((int)temp) + "C");
    
    // Soleil simulé
    tft->fillCircle(160, 165, 25, 0xFFE0);
    for (int i = 0; i < 8; i++) {
      float angle = i * PI / 4;
      int x1 = 160 + cos(angle) * 30;
      int y1 = 165 + sin(angle) * 30;
      int x2 = 160 + cos(angle) * 38;
      int y2 = 165 + sin(angle) * 38;
      tft->drawLine(x1, y1, x2, y2, 0xFFE0);
    }
    
    // Condition
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setTextSize(2);
    tft->setCursor(110, 210);
    tft->print(condition);
    
    // Humidité
    tft->setTextSize(1);
    tft->setCursor(10, 215);
    tft->print("Hum: " + String(humidity) + "%");
  }
  
  void update() override {
    if (!loaded && WiFi.status() == WL_CONNECTED) {
      loaded = true;
      // Ici vous pouvez ajouter un appel HTTP réel
      // Pour l'instant valeurs simulées
      temp = 22.0;
      humidity = 65;
      condition = "Ensoleillé";
      draw();
    }
  }
};

App* createWeather() { return new WeatherApp(); }