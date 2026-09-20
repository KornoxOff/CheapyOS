class NotesApp : public App {
private:
  String notes = "Bienvenue dans ESP32-OS!\nTouchez pour ajouter du texte.";
  int cursorY = 50;
  int selectedNote = 0;
  String noteList[5] = {"Note 1", "Note 2", "Note 3", "Idées", "TODO"};
  
public:
  NotesApp() : App("Notes", 0xFFE0) {}
  
  void draw() override {
    tft->fillScreen(0xFFFF);
    tft->fillRect(0, 20, 100, 220, 0xCE79);
    
    // Liste des notes
    tft->setTextColor(TFT_BLACK, 0xCE79);
    tft->setTextSize(1);
    for (int i = 0; i < 5; i++) {
      int y = 30 + i * 35;
      if (i == selectedNote) {
        tft->fillRect(0, y - 5, 100, 30, 0x7BEF);
      }
      tft->setCursor(10, y);
      tft->print(noteList[i]);
    }
    
    // Zone d'édition
    tft->setTextColor(TFT_BLACK, TFT_WHITE);
    tft->setCursor(110, 30);
    tft->print(notes);
    
    // Curseur clignotant
    tft->fillRect(110, 30 + cursorY - 50, 2, 12, TFT_BLACK);
  }
  
  void handleTouch(int x, int y) override {
    if (x < 100) {
      int idx = (y - 25) / 35;
      if (idx >= 0 && idx < 5) {
        selectedNote = idx;
        draw();
      }
    } else {
      // Ajout de texte (simulé - clavier tactile à implémenter)
      notes += "X";
      draw();
    }
  }
};

App* createNotes() { return new NotesApp(); }