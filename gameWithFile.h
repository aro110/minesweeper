#ifndef GAMEWITHFILE_H
#define GAMEWITHFILE_H
#include "gameWithFile.h"
#include "cell.h"
#include "instruction.h"
#include "score.h"
#include "initGame.h"
#include "printBoard.h"
int processFile(const char *filepath, int *score, int *moves, int *gameResult);
void handleFileInput(int argc, char *argv[]);
#endif //GAMEWITHFILE_H
