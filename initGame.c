#include "cell.h"
#include "score.h"
#include "initGame.h"
#include "printBoard.h"
#include <stdio.h>
#include <stdlib.h>

#define AND &&
#define OR ||
#define NOT !=

void placeMines(struct cell **board, int rows, int cols, int numMin, int safeX, int safeY) {
    int count = 0;
    printf("Rozmieszczam miny z wykluczeniem (%d, %d)...\n", safeX, safeY);

    while (count < numMin) {
        int x = rand() % rows;
        int y = rand() % cols;

        // sprawdzanie bezpieczenstwa pola
        if ((x >= safeX - 1 AND x <= safeX + 1) AND (y >= safeY - 1 AND y <= safeY + 1)) {
            continue;
        }

        if (board[x][y].isMine == 0) {
            board[x][y].isMine = 1;
            count++;
            // printf("Mina na: %d %d\n", x, y);
        }
    }
}

void initBoard(struct cell **board, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j].isMine = 0;
            board[i][j].status = 0;
            board[i][j].nearMineCount = 0;
        }
    }
}

void revealCell (struct cell **board, int x, int y, int rows, int cols, int multiplier, int *score) {
    if (x<0 OR y<0 OR x>=rows OR y>=cols OR board[x][y].status == 1 OR board[x][y].status == 2) return;

    board[x][y].status = 1;
    *score += multiplier;

    if (board[x][y].isMine) {
        printf("Koniec gry, mina!\nTwój końcowy wynik to: %d\n", *score);
        revealAllMines(board, rows, cols);
        writeScore(score);
        freeBoard(board, rows);
        exit(1);
    }

    if(board[x][y].nearMineCount == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx NOT 0 OR dy NOT 0) {
                    revealCell(board, x+dx, y+dy, rows, cols, multiplier, score);
                }
            }
        }
    }

}

void nearMines(struct cell **board, int rows, int cols) {
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            if(board[x][y].isMine) continue;

            int count = 0;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = x+dx;
                    int ny = y+dy;
                    if (nx >= 0 AND ny >= 0 AND nx < rows AND ny < cols AND board[nx][ny].isMine) count++;
                }
            }

            board[x][y].nearMineCount = count;
        }
    }
}