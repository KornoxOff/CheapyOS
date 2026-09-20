class MusicApp : public App {
private:
  bool playing = false;
  int track = 0;
  int progress = 0;
  String tracks[5] = {"Piste 1", "Piste 2", "Piste 3", "Piste 4", "Piste 5"};
  unsigned long lastTick = 0;
  
public:
  MusicApp() : App("Musique", 0xF81F) {}
  
  void draw() override {
    tft->fillScreen(0x0841);
    tft->setTextColor(TFT_WHITE, 0x0841);
    tft->setTextSize(2);
    tft->setCursor(10, 25);
    tft->print("Musique");
    
    // Album art
    tft->fillRoundRect(60, 60, 200, 80, 8, 0xF81F);
    tft->drawRoundRect(60, 60, 200, 80, 8, TFT_WHITE);
    
    // Titre
    tft->setTextSize(1);
    tft->setCursor(120, 150);
    tft->print(tracks[track]);
    
    // Barre de progression
    tft->drawRect(40, 170, 240, 6, TFT_WHITE);
    tft->fillRect(40, 170, progress * 240 / 100, 6, 0x07E0);
    
    // Boutons
    tft->fillCircle(80, 200, 20, 0x4208);
    tft->setTextColor(TFT_WHITE, 0x4208);
    tft->setCursor(76, 195);
    tft->print("<<");
    
    tft->fillCircle(160, 200, 25, playing ? 0xF800 : 0x07E0);
    tft->setTextColor(TFT_WHITE);
    tft->setCursor(155, 193);
    tft->print(playing ? "||" : ">");
    
    tft->fillCircle(240, 200, 20, 0x4208);
    tft->setTextColor(TFT_WHITE, 0x4208);
    tft->setCursor(236, 195);
    tft->print(">>");
  }
  
  void handleTouch(int x, int y) override {
    if (y > 175 && y < 225) {
      if (x > 60 && x < 100) { track = (track - 1 + 5) % 5; }
      else if (x > 135 && x < 185) { playing = !playing; }
      else if (x > 220 && x < 260) { track = (track + 1) % 5; }
      draw();
    }
  }
  
  void update() override {
    if (playing && millis() - lastTick > 500) {
      lastTick = millis();
      progress = (progress + 1) % 100;
      tft->fillRect(40, 170, 240, 6, TFT_WHITE);
      tft->fillRect(40, 170, progress * 240 / 100, 6, 0x07E0);
    }
  }
};

App* createMusic() { return new MusicApp(); }