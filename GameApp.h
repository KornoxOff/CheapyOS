class GameApp : public App {
private:
  struct Point { int x, y; };
  Point snake[50];
  int snakeLen = 3;
  int dir = 0; // 0=right, 1=down, 2=left, 3=up
  Point food;
  int score = 0;
  bool gameOver = false;
  unsigned long lastMove = 0;
  const int cellSize = 10;
  const int gridW = 30, gridH = 19;
  const int offsetY = 30;
  
public:
  GameApp() : App("Snake", 0xFFE0) {}
  
  void reset() {
    snakeLen = 3;
    snake[0] = {10, 10};
    snake[1] = {9, 10};
    snake[2] = {8, 10};
    dir = 0;
    score = 0;
    gameOver = false;
    spawnFood();
  }
  
  void spawnFood() {
    food.x = random(1, gridW - 1);
    food.y = random(1, gridH - 1);
  }
  
  void draw() override {
    tft->fillScreen(0x0841);
    tft->setTextColor(TFT_WHITE, 0x0841);
    tft->setTextSize(1);
    tft->setCursor(10, 5);
    tft->print("Score: " + String(score));
    tft->setCursor(150, 5);
    tft->print("Flèches pour diriger");
    
    // Grille
    tft->drawRect(0, offsetY, gridW * cellSize, gridH * cellSize, TFT_WHITE);
    
    // Serpent
    for (int i = 0; i < snakeLen; i++) {
      tft->fillRect(snake[i].x * cellSize + 1, 
                    snake[i].y * cellSize + offsetY + 1,
                    cellSize - 2, cellSize - 2,
                    i == 0 ? 0x07E0 : 0x0400);
    }
    
    // Nourriture
    tft->fillRect(food.x * cellSize + 1, food.y * cellSize + offsetY + 1,
                  cellSize - 2, cellSize - 2, 0xF800);
    
    if (gameOver) {
      tft->fillRect(80, 100, 160, 40, 0x0000);
      tft->drawRect(80, 100, 160, 40, 0xF800);
      tft->setTextColor(TFT_RED, 0x0000);
      tft->setTextSize(2);
      tft->setCursor(100, 115);
      tft->print("GAME OVER");
    }
    
    // Boutons directionnels
    tft->fillTriangle(40, 230, 60, 230, 50, 215, TFT_WHITE);
    tft->fillTriangle(150, 230, 170, 230, 160, 215, TFT_WHITE);
    tft->fillTriangle(90, 230, 110, 230, 100, 245, TFT_WHITE);
    tft->fillTriangle(90, 215, 110, 215, 100, 200, TFT_WHITE);
  }
  
  void handleTouch(int x, int y) override {
    if (gameOver) { reset(); draw(); return; }
    
    if (y > 200 && y < 245) {
      if (x < 80) dir = 2;
      else if (x > 130) dir = 0;
      else if (y > 225) dir = 3;
      else dir = 1;
    }
  }
  
  void update() override {
    if (gameOver || millis() - lastMove < 200) return;
    lastMove = millis();
    
    // Nouvelle tête
    Point head = snake[0];
    if (dir == 0) head.x++;
    else if (dir == 1) head.y++;
    else if (dir == 2) head.x--;
    else head.y--;
    
    // Collisions
    if (head.x < 0 || head.x >= gridW || head.y < 0 || head.y >= gridH) {
      gameOver = true; draw(); return;
    }
    for (int i = 0; i < snakeLen; i++) {
      if (snake[i].x == head.x && snake[i].y == head.y) {
        gameOver = true; draw(); return;
      }
    }
    
    // Décalage
    for (int i = snakeLen - 1; i > 0; i--) snake[i] = snake[i-1];
    snake[0] = head;
    
    // Manger
    if (head.x == food.x && head.y == food.y) {
      if (snakeLen < 50) snakeLen++;
      score += 10;
      spawnFood();
    }
    
    draw();
  }
};

App* createGame() { return new GameApp(); }