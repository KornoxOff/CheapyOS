#ifndef CALCULATORAPP_H
#define CALCULATORAPP_H

#include "App.h"

class CalculatorApp : public App {
private:
  String current = "0";
  float storedValue = 0;
  char lastOp = 0;
  bool newNumber = true;
  bool hasDecimal = false;

  // Couleurs
  uint16_t cDigit = 0x39E7;
  uint16_t cOperator = 0xF800;
  uint16_t cEquals = 0x07E0;
  uint16_t cFunction = 0x7BEF;
  uint16_t cClear = 0xFD20;

  // Positions des boutons (portrait 240x320)
  const int btnW = 54;
  const int btnH = 42;
  const int gapX = 4;
  const int gapY = 4;
  const int startX = 4;
  const int startY = 130;

  struct Button {
    String label;
    uint16_t color;
    char action;
  };

  Button buttons[19] = {
    // Ligne 1 : C, %, <, /
    {"C",  cClear,    'C'},
    {"%",  cFunction, '%'},
    {"<",  cFunction, 'B'},
    {"/",  cOperator, '/'},
    // Ligne 2 : 7, 8, 9, *
    {"7",  cDigit,    '7'},
    {"8",  cDigit,    '8'},
    {"9",  cDigit,    '9'},
    {"*",  cOperator, '*'},
    // Ligne 3 : 4, 5, 6, -
    {"4",  cDigit,    '4'},
    {"5",  cDigit,    '5'},
    {"6",  cDigit,    '6'},
    {"-",  cOperator, '-'},
    // Ligne 4 : 1, 2, 3, +
    {"1",  cDigit,    '1'},
    {"2",  cDigit,    '2'},
    {"3",  cDigit,    '3'},
    {"+",  cOperator, '+'},
    // Ligne 5 : 0, ., =
    {"0",  cDigit,    '0'},
    {".",  cDigit,    '.'},
    {"=",  cEquals,   '='}
  };

  int getButtonIndexAt(int x, int y) {
    for (int i = 0; i < 19; i++) {
      int col = i % 4;
      int row = i / 4;
      int bx = startX + col * (btnW + gapX);
      int by = startY + row * (btnH + gapY);

      // "=" (index 18) prend 2 cases en largeur
      if (i == 18) {
        if (x >= bx && x <= bx + btnW * 2 + gapX &&
            y >= by && y <= by + btnH) return i;
      } else {
        if (x >= bx && x <= bx + btnW &&
            y >= by && y <= by + btnH) return i;
      }
    }
    return -1;
  }

  void drawButton(int i) {
    int col = i % 4;
    int row = i / 4;
    int bx = startX + col * (btnW + gapX);
    int by = startY + row * (btnH + gapY);

    int w = (i == 18) ? btnW * 2 + gapX : btnW;

    tft->fillRoundRect(bx, by, w, btnH, 6, buttons[i].color);
    tft->drawRoundRect(bx, by, w, btnH, 6, tft->color565(255, 255, 255));

    tft->setTextColor(TFT_WHITE, buttons[i].color);
    tft->setTextSize(2);

    String label = buttons[i].label;
    int textW = label.length() * 12;
    int textH = 16;
    tft->setCursor(bx + (w - textW) / 2, by + (btnH - textH) / 2);
    tft->print(label);
  }

  void drawAllButtons() {
    for (int i = 0; i < 19; i++) {
      drawButton(i);
    }
  }

  void drawDisplay() {
    tft->fillRect(0, 24, 240, 100, 0x0841);

    tft->drawRoundRect(4, 30, 232, 90, 8, tft->color565(80, 100, 150));
    tft->fillRoundRect(5, 31, 230, 88, 7, 0x18C3);

    tft->setTextColor(TFT_WHITE, 0x18C3);
    tft->setTextSize(3);

    int textLen = current.length();
    int textW = textLen * 18;
    int textX = 220 - textW;
    if (textX < 10) textX = 10;

    tft->setCursor(textX, 70);
    tft->print(current);

    // Opération précédente
    if (lastOp != 0) {
      tft->setTextSize(1);
      tft->setTextColor(tft->color565(180, 200, 255), 0x18C3);
      tft->setCursor(10, 45);
      tft->print(String(storedValue, 2));
      tft->print(" ");
      tft->print(lastOp);
    }
  }

  void compute() {
    if (lastOp == 0) return;

    float val = current.toFloat();
    float result = storedValue;

    switch(lastOp) {
      case '+': result = storedValue + val; break;
      case '-': result = storedValue - val; break;
      case '*': result = storedValue * val; break;
      case '/':
        if (val == 0) {
          current = "Erreur";
          newNumber = true;
          lastOp = 0;
          return;
        }
        result = storedValue / val;
        break;
    }

    if (result == (int)result) {
      current = String((int)result);
    } else {
      current = String(result, 4);
      while (current.endsWith("0")) current.remove(current.length() - 1);
      if (current.endsWith(".")) current.remove(current.length() - 1);
    }

    storedValue = result;
    newNumber = true;
    lastOp = 0;
  }

  void handleButtonPress(char action) {
    // Chiffres
    if (action >= '0' && action <= '9') {
      if (newNumber) {
        current = String(action);
        newNumber = false;
      } else {
        if (current.length() < 10) {
          current += action;
        }
      }
    }
    // Point décimal
    else if (action == '.') {
      if (newNumber) {
        current = "0.";
        newNumber = false;
      } else if (current.indexOf(".") < 0) {
        current += ".";
      }
    }
    // Clear
    else if (action == 'C') {
      current = "0";
      storedValue = 0;
      lastOp = 0;
      newNumber = true;
    }
    // Backspace
    else if (action == 'B') {
      if (current.length() > 1) {
        current.remove(current.length() - 1);
      } else {
        current = "0";
        newNumber = true;
      }
    }
    // Pourcentage
    else if (action == '%') {
      float val = current.toFloat();
      val = val / 100.0;
      if (val == (int)val) current = String((int)val);
      else current = String(val, 4);
    }
    // Opérateurs
    else if (action == '+' || action == '-' || action == '*' || action == '/') {
      if (lastOp != 0 && !newNumber) {
        compute();
      } else {
        storedValue = current.toFloat();
      }
      lastOp = action;
      newNumber = true;
    }
    // Égal
    else if (action == '=') {
      if (lastOp != 0) {
        compute();
      }
    }

    drawDisplay();
  }

public:
  CalculatorApp() : App("Calculatrice", 0xF800) {}

  void setup(TFT_eSPI* display) override {
    App::setup(display);
    current = "0";
    storedValue = 0;
    lastOp = 0;
    newNumber = true;
  }

  void draw() override {
    tft->fillScreen(0x0841);
    drawDisplay();
    drawAllButtons();
  }

  void handleTouch(int x, int y) override {
    if (y < 24) return;

    int idx = getButtonIndexAt(x, y);
    if (idx >= 0) {
      handleButtonPress(buttons[idx].action);
    }
  }
};

App* createCalculator() { return new CalculatorApp(); }

#endif