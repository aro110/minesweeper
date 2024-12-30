#ifndef SCORE_H
#define SCORE_H
#include "gameWithFile.h"
#include "cell.h"
#include "instruction.h"
#include "score.h"
#include "initGame.h"
#include "printBoard.h"
void writeScore(int *score);
int diffLevel();
int checkWin(struct cell **board, int rows, int cols);
#endif //SCORE_H
