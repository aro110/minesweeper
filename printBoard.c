#include "printBoard.h"
#include "gameWithFile.h"
#include "cell.h"
#include "instruction.h"
#include "score.h"
#include "initGame.h"
#include <stdio.h>
#include "cell.h"

// funkcja wyswietlajaca plansze
void printBoard(struct cell **board, int rows, int cols) {
    // naglowki kolumn
    printf("    ");
    for (int i = 0; i < cols; i++) {
        printf("%2d  ", i+1);
    }
    printf("\n");

    // gorna ramka
    printf("   +");
    for (int i = 0; i < cols; i++) {
        printf("---+");
    }
    printf("\n");

    // zawartosc planszy
    for (int x = 0; x < rows; x++) {
        printf("%2d |", x+1); // Numer wiersza
        for (int y = 0; y < cols; y++) {
            if (board[x][y].status == 1) { // odkryte
                if (board[x][y].isMine) {
                    printf(" * |");
                } else {
                    if (board[x][y].nearMineCount == 0) printf("   |");
                    else printf(" %d |", board[x][y].nearMineCount);
                }
            } else if (board[x][y].status == 2) { // flaga
                printf(" P |");
            } else { // Pole zakryte
                printf("███|");
            }
        }
        printf("\n");

        // Ramka pozioma miedzy wierszami
        printf("   +");
        for (int i = 0; i < cols; i++) {
            printf("---+");
        }
        printf("\n");
    }
}

// stawianie flagi
void placeFlag(struct cell **board, int x, int y, int *remainingFlags) {

    // jezeli jest odkryte pole to nie mozna postawic flagi
    if (board[x][y].status == 1) {
        printf("Nie można postawić flagi na odkrytym polu.\n");
        return;
    }

    // stawianie i usuwanie flagi
    if (board[x][y].status == 0) {
        board[x][y].status = 2;
        *remainingFlags-=1;
    } else if (board[x][y].status == 2) {
        board[x][y].status = 0;
        *remainingFlags+=1;
    }
}

// odsloniecie wszystkich min w przypadku przegranej
void revealAllMines(struct cell **board, int rows, int cols) {
    // ustawienie kazdego pola na odkryte na ktorym jest mina
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j].isMine) {
                board[i][j].status = 1; // odkryj  miny
            }
        }
    }
    // wypisanie planszy z odkrytymi polami
    printBoard(board, rows, cols);
}
