#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameWithFile.h"
#include "cell.h"
#include "instruction.h"
#include "score.h"
#include "initGame.h"
#include "printBoard.h"

/* Wymagania:
 *3 poziomy trudności: latwy 9x9 pol 10 min; sredni 16x16 pol 40 min; trudny 16rz 30kol 99 min
 *f x y - ustaw flage
 *r _ _ odslon pole
 *pokazywac wynik, wynik liczy sie przez mnozenie liczby odslonietych pol razy mnoznik trudnosci (1 latwy, 2 sreni i 3 trudny)
 *zapisywac wynik i pokazywac 5 najlepszych (mysle ze w pliku do zapisania)
 *losowe rozmieszczenie min, w pierwszym ruchu nie mozna trafic na mine
 *wczytywac plansze i ruchy z pliku- program zwrwaca liczbe poprawnych krokow, liczbe punktow i czy sukces 1 czy fail 0
 *make do budowania aplikacji
 *make test do uruchomienia aplikacji
 *uzycie getopt do wpisywania argumentow wywolania aplikacji
 *sprawozdanie
 *repo na github
 *program podzielony na moduly
 *dodaj obsluge flag (nie mozna odkryc pola gdzie flaga i ilosc pozostalych flag)
 *AKTUALNY WYNIK PO KAZDYM RUCHU POKAZANY
 */

#define AND &&
#define OR ||
#define NOT !=

#define PAUSE system("pause")
#define CLEAR system("cls")

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "initGame.h"
#include "printBoard.h"
#include "gameWithFile.h"
#include "cell.h"

void testDiffLevel() {
    int diff = diffLevel();
    assert(diff == 1 || diff == 2 || diff == 3 || diff == 4);
    printf("Test wyboru poziomu trudnosci zakonczony sukcesem\n");
}

void testInitBoard() {
    int rows = 5, cols = 5;
    struct cell **board = malloc(rows * sizeof(struct cell *));
    for (int i = 0; i < rows; i++) {
        board[i] = malloc(cols * sizeof(struct cell));
    }

    initBoard(board, rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            assert(board[i][j].isMine == 0);
            assert(board[i][j].status == 0);
            assert(board[i][j].nearMineCount == 0);
        }
    }

    for (int i = 0; i < rows; i++) {
        free(board[i]);
    }
    free(board);

    printf("Test inicjalizacji planszy zakonczony sukcesem\n");
}

void testPlaceMines() {
    int rows = 5, cols = 5, numMines = 3;
    struct cell **board = malloc(rows * sizeof(struct cell *));
    for (int i = 0; i < rows; i++) {
        board[i] = malloc(cols * sizeof(struct cell));
    }
    initBoard(board, rows, cols);

    placeMines(board, rows, cols, numMines, 2, 2);

    int mineCount = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j].isMine) mineCount++;
        }
    }

    assert(mineCount == numMines);

    for (int i = 0; i < rows; i++) {
        free(board[i]);
    }
    free(board);

    printf("Test rozmieszczania min zakonczony sukcesem\n");
}

void testNearMines() {
    int rows = 5, cols = 5;
    struct cell **board = malloc(rows * sizeof(struct cell *));
    for (int i = 0; i < rows; i++) {
        board[i] = malloc(cols * sizeof(struct cell));
    }
    initBoard(board, rows, cols);

    placeMines(board, rows, cols, 3, 2, 2);
    nearMines(board, rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j].isMine == 0) {
                int count = 0;
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int nx = i + dx;
                        int ny = j + dy;
                        if (nx >= 0 && ny >= 0 && nx < rows && ny < cols && board[nx][ny].isMine) {
                            count++;
                        }
                    }
                }
                assert(board[i][j].nearMineCount == count);
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        free(board[i]);
    }
    free(board);

    printf("Test obliczania sasiedztwa min zakonczony sukcesem\n");
}

void testCheckWin() {
    int rows = 5, cols = 5;
    struct cell **board = malloc(rows * sizeof(struct cell *));
    for (int i = 0; i < rows; i++) {
        board[i] = malloc(cols * sizeof(struct cell));
    }
    initBoard(board, rows, cols);

    placeMines(board, rows, cols, 3, 2, 2);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j].status = board[i][j].isMine ? 2 : 1;
        }
    }

    assert(checkWin(board, rows, cols) == 1);

    for (int i = 0; i < rows; i++) {
        free(board[i]);
    }
    free(board);

    printf("Test sprawdzania wygranej zakonczony sukcesem\n");
}

void testInvalidArguments() {
    int rows = 5, cols = 5;
    struct cell **board = malloc(rows * sizeof(struct cell *));
    for (int i = 0; i < rows; i++) {
        board[i] = malloc(cols * sizeof(struct cell));
    }
    initBoard(board, rows, cols);

    printf("Test blednych argumentow\n");
    // Testowanie blednych wspolrzednych dla revealCell
    revealCell(board, -1, -1, rows, cols, 1, NULL);
    printf("RevealCell z nieprawidlowymi wspolrzednymi nie powoduje crashu\n");

    for (int i = 0; i < rows; i++) {
        free(board[i]);
    }
    free(board);

    printf("Test blednych argumentow zakonczony sukcesem\n");
}

void runTests() {
    testDiffLevel();
    testInitBoard();
    testPlaceMines();
    testNearMines();
    testCheckWin();
    testInvalidArguments();

    printf("\n=== WSZYSTKIE TESTY ZAKONCZONE SUKCESEM ===\n");
}


// zwolnienie pamieci
void freeBoard(struct cell **board, int rows) {
    for (int i = 0; i < rows; i++) {
        free(board[i]);
    }
    free(board);
}

void minesweeper() {
    int userInputX, userInputY, firstInput = 1, rows, cols, numMin, remainingFlags, score = 0;
    char command;

    // poziom trudnosci
    int diff = diffLevel(); // 1 latwy, 2 sredni, 3 trudny
    switch (diff) {
        case 1:
            rows = 9;
            cols = 9;
            numMin = 10;
            remainingFlags = numMin;
            break;
        case 2:
            rows = 16;
            cols = 16;
            numMin = 40;
            remainingFlags = numMin;
            break;
        case 3:
            rows = 16;
            cols = 30;
            numMin = 99;
            remainingFlags = numMin;
            break;
        case 4:
            printf("Podaj liczbe rzedow: ");
            while (scanf(" %d", &rows) != 1 || rows <= 0) {
                printf("Blad: Podaj poprawna liczbe rzedow (liczba calkowita dodatnia): ");
                while (getchar() != '\n'); // Czyszczenie bufora wejściowego
            }

            printf("Podaj liczbe kolumn: ");
            while (scanf(" %d", &cols) != 1 || cols <= 0) {
                printf("Blad: Podaj poprawna liczbe kolumn (liczba calkowita dodatnia): ");
                while (getchar() != '\n'); // Czyszczenie bufora wejściowego
            }

            int area = cols * rows;
            do {
                printf("Podaj liczbe min: ");
                if (scanf(" %d", &numMin) != 1 || numMin <= 0) {
                    printf("Blad: Podaj poprawna liczbe min (liczba calkowita dodatnia):\n");
                    while (getchar() != '\n'); // Czyszczenie bufora wejściowego
                    continue;
                }
                if (numMin > (area - 3)) {
                    printf("Liczba min jest zbyt duza. Maksymalna liczba min: %d\n", area - 3);
                }
            } while (numMin > (area - 3) || numMin <= 0);

            remainingFlags = numMin;
            break;
    }

    struct cell **board = malloc(sizeof(struct cell *) * rows);
    if (board == NULL) {
        printf("Blad alokacji pamieci.\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        board[i] = malloc(sizeof(struct cell) * cols);
        if (board[i] == NULL) {
            printf("Blad alokacji pamieci.\n");
            exit(1);
        }
    }

    initBoard(board, rows, cols); // inicjalizowanie planszy tzn ustawienie wszystkich wartosci pol na 0

    while (1) {
        printBoard(board, rows, cols); // wypisanie planszy
        printf("\nAktualny wynik: %d\nIlosc pozostalych flag: %d\n", score, remainingFlags);

        // wprowadzanie komend
        printf("Wprowadz wspolrzedne do odsloniecia (r/f x y): ");
        if (scanf(" %c %d %d", &command, &userInputX, &userInputY) NOT 3) {
            printf("Blad wprowadzania danych. Sprobuj ponownie.\n");
            while (getchar() NOT '\n'); // Czyszczenie bufora wejścia
            continue;
        }

        int x = userInputX - 1;
        int y = userInputY - 1;

        if (x < 0 || x >= rows || y < 0 || y >= cols) {
            printf("Nieprawidlowe wspolrzedne.\n");
            continue;
        }

        // po pierwszym ruchu rozlozenie min i odkrycie pol
        if (firstInput) {
            placeMines(board, rows, cols, numMin, x, y);
            nearMines(board, rows, cols);
            firstInput = 0;
        }

        // dzialanie komend
        if (command == 'r') { // odkrycie
            revealCell(board,x, y, rows, cols, diff, &score);
        } else if (command == 'f') { // flaga
            if (remainingFlags > 0) placeFlag(board, x, y, &remainingFlags);
            else printf("Uzyto wszystkie flagi, zdejmij jakas z planszy, aby postawic nowa.\n");
        } else { // blad
            printf("Blad wprowadzania danych\n");
        }

        // sprawdzenie wygranej
        if (checkWin(board, rows, cols)) {
            printf("Koniec gry, wygrales!\nTwoj wynik koncowy to: %d\n", score);
            writeScore(&score);
            freeBoard(board, rows);
            exit(0);
        }
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc > 1) {
        handleFileInput(argc, argv);
        exit(1);
    }

    int choice;

    do {
        printf("Witaj w grze Saper!\nWpisz liczbe, aby przejsc dalej:\n1. Zagraj.\n2. Pokaz instrukcje.\n3. Przykladowy plik.");
        scanf("%d", &choice);
        if (choice == 1) {
            minesweeper();
        } else if (choice == 2) {
            instruction();
        } else if (choice == 3) {
            fileExample();
        } else {
            printf("Blad wprowadzania danych.\n");
        }
    } while (choice != 1);

    PAUSE;
    return 0;
}
