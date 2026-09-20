class GalleryApp : public App {
private:
  int currentImage = 0;
  const int totalImages = 6;
  uint16_t colors[6] = {0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF};
  
public:
  GalleryApp() : App("Galerie", 0x07FF) {}
  
  void draw() override {
    tft->fillScreen(0x0841);
    tft->setTextColor(TFT_WHITE, 0x0841);
    tft->setTextSize(2);
    tft->setCursor(10, 25);
    tft->print("Galerie");
    
    // Image principale (dégradé simulé)
    int x0 = 40, y0 = 55, w = 240, h = 140;
    for (int i = 0; i < h; i++) {
      uint16_t c = tft->alphaBlend(map(i, 0, h, 0, 255), colors[currentImage], TFT_BLACK);
      tft->drawFastHLine(x0, y0 + i, w, c);
    }
    tft->drawRect(x0, y0, w, h, TFT_WHITE);
    
    // Numéro
    tft->setTextSize(1);
    tft->setCursor(140, 200);
    tft->print(String(currentImage + 1) + " / " + String(totalImages));
    
    // Flèches
    tft->fillTriangle(20, 125, 35, 105, 35, 145, TFT_WHITE);
    tft->fillTriangle(300, 125, 285, 105, 285, 145, TFT_WHITE);
    
    // Vignettes
    for (int i = 0; i < totalImages; i++) {
      int vx = 55 + i * 35;
      tft->fillRect(vx, 215, 30, 20, colors[i]);
      if (i == currentImage) tft->drawRect(vx, 215, 30, 20, TFT_WHITE);
    }
  }
  
  void handleTouch(int x, int y) override {
    if (x < 40 && y > 100 && y < 150) {
      currentImage = (currentImage - 1 + totalImages) % totalImages;
      draw();
    } else if (x > 280 && y > 100 && y < 150) {
      currentImage = (currentImage + 1) % totalImages;
      draw();
    } else if (y > 210) {
      int idx = (x - 55) / 35;
      if (idx >= 0 && idx < totalImages) {
        currentImage = idx;
        draw();
      }
    }
  }
};

App* createGallery() { return new GalleryApp(); }