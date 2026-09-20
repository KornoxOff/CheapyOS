#ifndef FILEMANAGERAPP_H
#define FILEMANAGERAPP_H

#include "App.h"
#include <SPIFFS.h>
#include <SD.h>

class FileManagerApp : public App {
private:
  String currentPath = "/";
  String entries[20];
  bool isDir[20];
  int sizes[20];
  int entryCount = 0;
  int selected = 0;
  int scroll = 0;
  bool sdAvailable = false;
  bool spiffsAvailable = false;
  int currentDrive = 0; // 0 = SPIFFS, 1 = SD

  // ================================
  // CHARGEMENT DU RÉPERTOIRE
  // ================================
  void loadDirectory() {
    entryCount = 0;

    File root = (currentDrive == 0) ? SPIFFS.open(currentPath) : SD.open(currentPath);
    if (!root || !root.isDirectory()) return;

    File file = root.openNextFile();
    while (file && entryCount < 20) {
      String name = String(file.name());
      int lastSlash = name.lastIndexOf('/');
      if (lastSlash >= 0) name = name.substring(lastSlash + 1);

      entries[entryCount] = name;
      isDir[entryCount] = file.isDirectory();
      sizes[entryCount] = file.size();
      entryCount++;
      file = root.openNextFile();
    }
    root.close();
  }

  // ================================
  // FORMATAGE TAILLE
  // ================================
  String formatSize(int bytes) {
    if (bytes < 1024) return String(bytes) + "B";
    if (bytes < 1024 * 1024) return String(bytes / 1024) + "KB";
    return String(bytes / (1024 * 1024)) + "MB";
  }

  // ================================
  // ICÔNE FICHIER/DOSSIER
  // ================================
  void drawFileIcon(int x, int y, bool dir) {
    if (dir) {
      tft->fillRect(x, y + 2, 12, 10, 0xFFE0);
      tft->fillRect(x, y, 6, 3, 0xFFE0);
      tft->drawRect(x, y + 2, 12, 10, 0x7BEF);
    } else {
      tft->fillRect(x, y, 10, 13, TFT_WHITE);
      tft->drawRect(x, y, 10, 13, 0x7BEF);
      tft->drawLine(x + 6, y, x + 10, y + 4, 0x7BEF);
      tft->drawLine(x + 6, y, x + 6, y + 4, 0x7BEF);
      tft->drawLine(x + 6, y + 4, x + 10, y + 4, 0x7BEF);
    }
  }

  // ================================
  // EN-TÊTE (chemin + drive)
  // ================================
  void drawHeader() {
    tft->fillRect(0, 22, 320, 24, 0x2104);

    tft->setTextColor(TFT_WHITE, 0x2104);
    tft->setTextSize(1);
    tft->setCursor(5, 29);
    tft->print(currentDrive == 0 ? "[SPIFFS]" : "[SD]");

    tft->setCursor(60, 29);
    tft->print(currentPath);

    tft->setCursor(295, 29);
    tft->print(scroll > 0 ? "^" : " ");
  }

  // ================================
  // LISTE DES FICHIERS
  // ================================
  void drawFileList() {
    tft->fillRect(0, 46, 320, 150, 0x0841);

    for (int i = scroll; i < entryCount && i < scroll + 6; i++) {
      int y = 48 + (i - scroll) * 25;

      if (i == selected) {
        tft->fillRect(0, y, 320, 24, 0x001F);
      }

      drawFileIcon(8, y + 5, isDir[i]);

      tft->setTextColor(TFT_WHITE, (i == selected) ? 0x001F : 0x0841);
      tft->setTextSize(1);
      tft->setCursor(28, y + 8);

      String name = entries[i];
      if (name.length() > 22) name = name.substring(0, 20) + "..";
      tft->print(name);

      tft->setCursor(240, y + 8);
      if (isDir[i]) {
        tft->print("<DIR>");
      } else {
        tft->print(formatSize(sizes[i]));
      }
    }

    if (entryCount == 0) {
      tft->setTextColor(0x7BEF, 0x0841);
      tft->setCursor(100, 100);
      tft->print("Dossier vide");
    }
  }

  // ================================
  // BARRE INFÉRIEURE
  // ================================
  void drawBottomBar() {
    int y = 200;
    tft->fillRect(0, y, 320, 40, 0x2104);
    tft->drawFastHLine(0, y, 320, 0x4208);

    tft->fillRoundRect(5, y + 5, 65, 30, 5, 0x4208);
    tft->setTextColor(TFT_WHITE, 0x4208);
    tft->setTextSize(1);
    tft->setCursor(20, y + 16);
    tft->print("< RETOUR");

    tft->fillRoundRect(78, y + 5, 75, 30, 5, 0xF800);
    tft->setTextColor(TFT_WHITE, 0xF800);
    tft->setCursor(95, y + 16);
    tft->print("EFFACER");

    tft->fillRoundRect(160, y + 5, 75, 30, 5, 0x07E0);
    tft->setTextColor(TFT_BLACK, 0x07E0);
    tft->setCursor(178, y + 16);
    tft->print("NOUVEAU");

    tft->fillRoundRect(242, y + 5, 73, 30, 5, 0x001F);
    tft->setTextColor(TFT_WHITE, 0x001F);
    tft->setCursor(253, y + 16);
    tft->print("DISQUE");
  }

  // ================================
  // NAVIGATION
  // ================================
  void goBack() {
    if (currentPath == "/") return;

    int lastSlash = currentPath.lastIndexOf('/');
    if (lastSlash <= 0) {
      currentPath = "/";
    } else {
      currentPath = currentPath.substring(0, lastSlash);
    }
    selected = 0;
    scroll = 0;
    draw();
  }

  // ================================
  // OUVRIR UN FICHIER/DOSSIER
  // ================================
  void openEntry(int idx) {
    if (isDir[idx]) {
      if (currentPath == "/") {
        currentPath = "/" + entries[idx];
      } else {
        currentPath += "/" + entries[idx];
      }
      selected = 0;
      scroll = 0;
      draw();
    } else {
      showFilePreview(entries[idx]);
    }
  }

  // ================================
  // APERÇU DE FICHIER
  // ================================
  void showFilePreview(String filename) {
    tft->fillScreen(0x0841);

    tft->fillRect(0, 22, 320, 24, 0x2104);
    tft->setTextColor(TFT_WHITE, 0x2104);
    tft->setTextSize(1);
    tft->setCursor(5, 29);
    tft->print("Apercu: " + filename);

    String fullPath = (currentPath == "/") ? "/" + filename : currentPath + "/" + filename;
    File f = (currentDrive == 0) ? SPIFFS.open(fullPath) : SD.open(fullPath);

    if (f) {
      tft->setTextColor(TFT_WHITE, 0x0841);
      tft->setCursor(10, 60);

      int lineY = 60;
      int charsOnLine = 0;
      while (f.available() && lineY < 190) {
        char c = f.read();
        if (c == '\n' || charsOnLine > 50) {
          lineY += 12;
          tft->setCursor(10, lineY);
          charsOnLine = 0;
        } else if (c >= 32 && c < 127) {
          tft->write(c);
          charsOnLine++;
        }
      }
      f.close();
    }

    tft->fillRoundRect(10, 205, 80, 25, 5, 0x4208);
    tft->setTextColor(TFT_WHITE, 0x4208);
    tft->setCursor(30, 213);
    tft->print("< RETOUR");

    // Attendre le toucher (adapté CYD)
    delay(100);
    while (true) {
      uint16_t tx, ty;
      if (tft->getTouch(&tx, &ty)) {
        if (ty > 200) break;
      }
      delay(50);
    }
    draw();
  }

  // ================================
  // SUPPRIMER
  // ================================
  void deleteSelected() {
    if (entryCount == 0) return;

    String fullPath = (currentPath == "/") ? "/" + entries[selected]
                                            : currentPath + "/" + entries[selected];

    bool ok;
    if (currentDrive == 0) {
      ok = isDir[selected] ? SPIFFS.rmdir(fullPath) : SPIFFS.remove(fullPath);
    } else {
      ok = isDir[selected] ? SD.rmdir(fullPath) : SD.remove(fullPath);
    }

    tft->fillRect(100, 100, 120, 30, ok ? 0x07E0 : 0xF800);
    tft->setTextColor(TFT_WHITE);
    tft->setTextSize(1);
    tft->setCursor(115, 110);
    tft->print(ok ? "Supprime" : "Erreur");
    delay(600);

    if (selected >= entryCount - 1) selected = max(0, entryCount - 2);
    draw();
  }

  // ================================
  // CRÉER UN FICHIER
  // ================================
  void createNewFile() {
    String name = "nouveau";
    int n = 1;
    String fullPath;

    do {
      fullPath = (currentPath == "/") ? "/" + name + String(n) + ".txt"
                                       : currentPath + "/" + name + String(n) + ".txt";
      n++;
    } while (((currentDrive == 0) ? SPIFFS.exists(fullPath) : SD.exists(fullPath)) && n < 100);

    File f = (currentDrive == 0) ? SPIFFS.open(fullPath, FILE_WRITE) : SD.open(fullPath, FILE_WRITE);
    if (f) {
      f.println("Fichier cree par ESP32-OS");
      f.println("Date: " + String(millis()));
      f.close();

      tft->fillRect(90, 100, 140, 30, 0x07E0);
      tft->setTextColor(TFT_WHITE);
      tft->setCursor(105, 110);
      tft->print("Fichier cree!");
      delay(600);
    }
    draw();
  }

  // ================================
  // CHANGER DE DISQUE
  // ================================
  void switchDrive() {
    if (!sdAvailable && !spiffsAvailable) return;

    if (currentDrive == 0 && sdAvailable) {
      currentDrive = 1;
    } else if (currentDrive == 1 && spiffsAvailable) {
      currentDrive = 0;
    }

    currentPath = "/";
    selected = 0;
    scroll = 0;
    draw();
  }

public:
  FileManagerApp() : App("Fichiers", 0xFD20) {}

  void setup(TFT_eSPI* display) override {
    App::setup(display);

    if (!SPIFFS.begin(true)) {
      Serial.println("SPIFFS failed");
    } else {
      spiffsAvailable = true;
    }

    if (SD.begin(5)) {
      sdAvailable = true;
    }

    currentDrive = spiffsAvailable ? 0 : (sdAvailable ? 1 : 0);
  }

  // ================================
  // DESSIN PRINCIPAL
  // ================================
  void draw() override {
    tft->fillScreen(0x0841);

    loadDirectory();
    drawHeader();
    drawFileList();
    drawBottomBar();

    if (!spiffsAvailable && !sdAvailable) {
      tft->setTextColor(TFT_RED, 0x0841);
      tft->setTextSize(2);
      tft->setCursor(40, 120);
      tft->print("Aucun stockage");
    }
  }

  // ================================
  // GESTION DU TOUCHER
  // ================================
  void handleTouch(int x, int y) override {
    if (y >= 48 && y < 198) {
      int idx = scroll + (y - 48) / 25;
      if (idx >= 0 && idx < entryCount) {
        if (selected == idx) {
          openEntry(idx);
        } else {
          selected = idx;
          draw();
        }
      }
      return;
    }

    if (y >= 205) {
      if (x < 70) {
        goBack();
      } else if (x < 155) {
        deleteSelected();
      } else if (x < 237) {
        createNewFile();
      } else {
        switchDrive();
      }
    }
  }
};

App* createFiles() { return new FileManagerApp(); }

#endif