#ifndef TICTACTOEAPP_H
#define TICTACTOEAPP_H

#include "App.h"

class TicTacToeApp : public App {
private:
  int board[9];
  bool playerTurn = true;
  int winner = 0;  // 0 = en cours, 1 = joueur, 2 = IA, 3 = match nul

  void reset() {
    for (int i = 0; i < 9; i++) board[i] = 0;
    playerTurn = true;
    winner = 0;
  }

  void checkWinner() {
    int lines[8][3] = {
      {0,1,2}, {3,4,5}, {6,7,8},
      {0,3,6}, {1,4,7}, {2,5,8},
      {0,4,8}, {2,4,6}
    };
    for (int i = 0; i < 8; i++) {
      int a = lines[i][0], b = lines[i][1], c = lines[i][2];
      if (board[a] != 0 && board[a] == board[b] && board[b] == board[c]) {
        winner = board[a];
        return;
      }
    }
    bool full = true;
    for (int i = 0; i < 9; i++) if (board[i] == 0) full = false;
    if (full) winner = 3;
  }

  void aiMove() {
    // IA simple : joue une case libre au hasard
    int available[9];
    int n = 0;
    for (int i = 0; i < 9; i++) if (board[i] == 0) available[n++] = i;
    if (n > 0) {
      board[available[random(n)]] = 2;
    }
  }

public:
  TicTacToeApp() : App("Morpion", 0xFD20) { reset(); }

  void draw() override {
    tft->fillScreen(0x0841);

    tft->setTextColor(TFT_WHITE, 0x0841);
    tft->setTextSize(2);
    tft->setCursor(40, 30);
    tft->print("Morpion");

    // Grille
    int startX = 30, startY = 80, cellSize = 60;
    for (int i = 0; i < 9; i++) {
      int col = i % 3;
      int row = i / 3;
      int x = startX + col * cellSize;
      int y = startY + row * cellSize;

      tft->drawRect(x, y, cellSize, cellSize, TFT_WHITE);

      if (board[i] == 1) {
        // X (bleu)
        tft->drawLine(x + 15, y + 15, x + 45, y + 45, 0x07FF);
        tft->drawLine(x + 45, y + 15, x + 15, y + 45, 0x07FF);
        tft->drawLine(x + 16, y + 15, x + 46, y + 45, 0x07FF);
        tft->drawLine(x + 44, y + 15, x + 14, y + 45, 0x07FF);
      } else if (board[i] == 2) {
        // O (rouge)
        tft->drawCircle(x + 30, y + 30, 15, 0xF800);
        tft->drawCircle(x + 30, y + 30, 14, 0xF800);
      }
    }

    // Statut
    tft->setTextSize(1);
    tft->setCursor(20, 280);
    if (winner == 0) {
      tft->setTextColor(TFT_WHITE, 0x0841);
      tft->print("A vous de jouer !");
    } else if (winner == 1) {
      tft->setTextColor(0x07E0, 0x0841);
      tft->print("VOUS AVEZ GAGNE !");
    } else if (winner == 2) {
      tft->setTextColor(0xF800, 0x0841);
      tft->print("IA a gagne !");
    } else {
      tft->setTextColor(0xFFE0, 0x0841);
      tft->print("Match nul !");
    }

    // Bouton reset
    tft->fillRoundRect(160, 275, 70, 25, 6, 0x4208);
    tft->setTextColor(TFT_WHITE, 0x4208);
    tft->setCursor(178, 283);
    tft->print("RESET");
  }

  void handleTouch(int x, int y) override {
    // Reset
    if (x >= 160 && x <= 230 && y >= 275 && y <= 300) {
      reset();
      draw();
      return;
    }

    if (winner != 0) return;
    if (!playerTurn) return;

    // Case touchée
    int startX = 30, startY = 80, cellSize = 60;
    for (int i = 0; i < 9; i++) {
      int col = i % 3;
      int row = i / 3;
      int cx = startX + col * cellSize;
      int cy = startY + row * cellSize;

      if (x >= cx && x <= cx + cellSize && y >= cy && y <= cy + cellSize) {
        if (board[i] == 0) {
          board[i] = 1;
          playerTurn = false;
          checkWinner();
          draw();
          if (winner == 0) {
            delay(500);
            aiMove();
            checkWinner();
            playerTurn = true;
            draw();
          }
        }
        return;
      }
    }
  }
};

App* createTicTacToe() { return new TicTacToeApp(); }
#endif