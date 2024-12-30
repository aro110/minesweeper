#ifndef INITGAME_H
#define INITGAME_H
#include "gameWithFile.h"
#include "cell.h"
#include "instruction.h"
#include "score.h"
#include "initGame.h"
#include "printBoard.h"
void placeMines(struct cell **board, int rows, int cols, int numMin, int safeX, int safeY);
void initBoard(struct cell **board, int rows, int cols);
void revealCell (struct cell **board, int x, int y, int rows, int cols, int multiplier, int *score);
void nearMines(struct cell **board, int rows, int cols);
#endif //INITGAME_H
