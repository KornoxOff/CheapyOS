#ifndef APP_H
#define APP_H

#include <TFT_eSPI.h>

class App {
public:
  String name;
  uint16_t iconColor;
  TFT_eSPI* tft;
  
  App(String n, uint16_t color) : name(n), iconColor(color) {}
  virtual ~App() {}
  
  virtual void setup(TFT_eSPI* display) { tft = display; }
  virtual void draw() = 0;
  virtual void handleTouch(int x, int y) {}
  virtual void update() {}
  virtual void exit() {}
};

#endif