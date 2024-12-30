#ifndef PRINTBOARD_H
#define PRINTBOARD_H
#include "gameWithFile.h"
#include "cell.h"
#include "instruction.h"
#include "score.h"
#include "initGame.h"
#include "printBoard.h"
void printBoard(struct cell **board, int rows, int cols);
void placeFlag(struct cell **board, int x, int y, int *remainingFlags);
void revealAllMines(struct cell **board, int rows, int cols);
#endif //PRINTBOARD_H
