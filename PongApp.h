#ifndef PONGAPP_H
#define PONGAPP_H

#include "App.h"

class PongApp : public App {
private:
  int ballX = 120, ballY = 160;
  int ballDX = 2, ballDY = 2;
  int paddleY = 140;
  int aiY = 140;
  int scorePlayer = 0, scoreAI = 0;
  unsigned long lastMove = 0;

public:
  PongApp() : App("Pong", 0x07FF) {}

  void draw() override {
    tft->fillScreen(TFT_BLACK);

    // Score
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setTextSize(3);
    tft->setCursor(40, 30);
    tft->print(scorePlayer);
    tft->setCursor(180, 30);
    tft->print(scoreAI);

    // Filet central
    for (int y = 80; y < 320; y += 15) {
      tft->drawFastVLine(120, y, 8, 0x4208);
    }

    // Raquette joueur (gauche)
    tft->fillRoundRect(10, paddleY, 6, 50, 3, 0x07E0);

    // Raquette IA (droite)
    tft->fillRoundRect(224, aiY, 6, 50, 3, 0xF800);

    // Balle
    tft->fillCircle(ballX, ballY, 5, TFT_WHITE);
  }

  void handleTouch(int x, int y) override {
    paddleY = constrain(y - 25, 80, 270);
    draw();
  }

  void update() override {
    if (millis() - lastMove < 30) return;
    lastMove = millis();

    ballX += ballDX;
    ballY += ballDY;

    // Rebonds murs
    if (ballY <= 80 || ballY >= 315) ballDY = -ballDY;

    // Raquette joueur
    if (ballX <= 20 && ballY >= paddleY && ballY <= paddleY + 50) {
      ballDX = abs(ballDX);
      ballX = 21;
    }

    // Raquette IA
    if (ballX >= 220 && ballY >= aiY && ballY <= aiY + 50) {
      ballDX = -abs(ballDX);
      ballX = 219;
    }

    // IA suit la balle
    if (ballY > aiY + 25) aiY += 2;
    else if (ballY < aiY + 25) aiY -= 2;
    aiY = constrain(aiY, 80, 270);

    // Score
    if (ballX < 0) {
      scoreAI++;
      ballX = 120; ballY = 160; ballDX = 2; ballDY = 2;
    }
    if (ballX > 240) {
      scorePlayer++;
      ballX = 120; ballY = 160; ballDX = -2; ballDY = 2;
    }

    draw();
  }
};

App* createPong() { return new PongApp(); }
#endif