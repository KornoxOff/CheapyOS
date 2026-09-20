const char* WIFI_SSID = "Freebox_LE GAL_";
const char* WIFI_PASSWORD = "Q.}378MbS$dg8W)Wp*_2E^Vwe43cn4";

#include <TFT_eSPI.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <Preferences.h>

#include "App.h"
#include "CalculatorApp.h"
#include "SettingsApp.h"
#include "ClockApp.h"
#include "NotesApp.h"
#include "WiFiApp.h"
#include "MusicApp.h"
#include "GalleryApp.h"
#include "WeatherApp.h"
#include "GameApp.h"
#include "FileManagerApp.h"
#include "Launcher.h"
#include "System.h"
#include "CalendarApp.h"
#include "AlarmApp.h"
#include "CameraApp.h"
#include "PaintApp.h"
#include "PongApp.h"
#include "TicTacToeApp.h"
#include "SysInfoApp.h"
#include "ConverterApp.h"
#include "DiceApp.h"
#include "BookApp.h"

TFT_eSPI tft = TFT_eSPI();
SystemOS os(&tft);

bool wifiShown = false;
unsigned long wifiStartTime = 0;

void setup() {
  Serial.begin(115200);

  // Backlight
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  SPIFFS.begin(true);

  // ⚡ WiFi en ARRIÈRE-PLAN (NON bloquant)
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  wifiStartTime = millis();
  Serial.print("WiFi en arriere-plan...");

  // L'OS démarre IMMÉDIATEMENT
  os.begin();
  os.boot();
}

void loop() {
  // Afficher quand WiFi prêt
  if (!wifiShown && WiFi.status() == WL_CONNECTED) {
    wifiShown = true;
    Serial.println("\nWiFi OK !");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Temps: ");
    Serial.print((millis() - wifiStartTime) / 1000);
    Serial.println("s");
  }

  // Timeout après 15s
  if (!wifiShown && millis() - wifiStartTime > 15000) {
    wifiShown = true;
    Serial.println("\nWiFi timeout");
  }

  os.update();
  delay(5);
}