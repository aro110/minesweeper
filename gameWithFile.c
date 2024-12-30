#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "gameWithFile.h"
#include "cell.h"
#include "score.h"
#include "initGame.h"

int processFile(const char *filepath, int *score, int *moves, int *gameResult) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Nie mozna otworzyc pliku: %s\n", filepath);
        return -1;
    }

    int rows, cols, numMines;
    char line[256]; // Bufor na jedną linię
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Blad. Nie udalo sie wczytac pierwszej linii.\n");
        fclose(file);
        return -1;
    }
    if (sscanf(line, "%d %d %d", &rows, &cols, &numMines) != 3) {
        fprintf(stderr, "Blad. Niepoprawny format pierwszej linii.\n");
        fclose(file);
        return -1;
    }

    int remainingFlags = numMines;

    struct cell **board = malloc(sizeof(struct cell *) * rows);
    for (int i = 0; i < rows; i++) {
        board[i] = malloc(sizeof(struct cell) * cols);
    }

    // inicjalizacja planszy
    initBoard(board, rows, cols);

    // Inicjalizacja zmiennych do walidacji
    int mineCount = 0; // Licznik min
    int lineCount = 0; // Licznik wierszy w planszy

    // Wczytanie planszy z pliku
    for (int i = 0; i < rows; i++) {
        lineCount++;
        for (int j = 0; j < cols; j++) {
            if (fscanf(file, "%d", &board[i][j].isMine) != 1) {
                fprintf(stderr, "Blad. Niekompletna plansza (wiersz %d, kolumna %d).\n", i + 1, j + 1);
                fclose(file);
                freeBoard(board, rows);
                return -1;
            }
            if (board[i][j].isMine == 1) {
                mineCount++;
            }
        }
    }

    // Sprawdzanie zgodności liczby wierszy
    if (lineCount != rows) {
        fprintf(stderr, "Blad. Liczba wierszy w pliku nie zgadza sie z zadeklarowana (%d zamiast %d).\n", lineCount, rows);
        fclose(file);
        freeBoard(board, rows);
        return -1;
    }

    // Sprawdzanie liczby min
    if (mineCount != numMines) {
        fprintf(stderr, "Blad. Liczba min na planszy (%d) nie zgadza sie z zadeklarowana (%d).\n", mineCount, numMines);
        fclose(file);
        freeBoard(board, rows);
        return -1;
    }

    // Oblicz pola wokol min
    nearMines(board, rows, cols);

    char command;
    int x, y;
    *moves = 0;
    *gameResult = 1; // Domyślnie sukces
    *score = 0;

    while (fscanf(file, " %c %d %d", &command, &x, &y) == 3) {
        x--;
        y--;
        (*moves)++;

        if (x < 0 || x >= rows || y < 0 || y >= cols) {
            fprintf(stderr, "Nieprawidlowe wspolrzedne w pliku na ruchu %d %d.\n", x+1, y+1);
            freeBoard(board, rows);
            fclose(file);
            return -1;
        }

        if (command == 'r') {
            if (board[x][y].isMine) {
                *gameResult = 0; // Przegrana
                break;
            }

            if (board[x][y].status == 0) {
                revealCell(board, x, y, rows, cols, 1, score);
            }

        } else if (command == 'f') {
            placeFlag(board, x, y, &remainingFlags);
        }
    }

    // if (*gameResult == 1 && !checkWin(board, rows, cols)) {
    //     printf("Plik jest niekompletny - gra nie zostala rozwiazana.\n");
    //     exit(1);
    // }

    fclose(file);

    freeBoard(board, rows);

    return 0;
}

void handleFileInput(int argc, char *argv[]) {
    int opt;
    char *filepath = NULL;

    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
            case 'f':
                filepath = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s -f <filepath>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (filepath == NULL) {
        fprintf(stderr, "Musisz podac sciezke do pliku za pomoca opcji -f\n");
        exit(EXIT_FAILURE);
    }

    int moves, gameResult, score;
    if (processFile(filepath, &score, &moves, &gameResult) == 0) {
        printf("Liczba poprawnych ruchow: %d\n", moves);
        printf("Liczba punktow: %d\n", score);
        printf("Wynik gry: %s\n", gameResult ? "Sukces" : "Porazka");
    }
}