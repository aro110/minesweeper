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

void freeBoard(struct cell **board, int rows) {
    for (int i = 0; i < rows; i++) {
        free(board[i]);
    }
    free(board);
}

void minesweeper() {
    int userInputX;
    int userInputY;
    int firstInput = 1;
    int rows, cols;
    char command;
    int numMin;
    int remainingFlags;
    int score = 0;

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
            scanf(" %d", &rows);
            printf("Podaj liczbe kolumn: ");
            scanf(" %d", &cols);
            int area = cols * rows;
            do {
                printf("Podaj liczbe min: ");
                scanf(" %d", &numMin);
            } while (area <= numMin);
            remainingFlags = numMin;
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

        if (command == 'r') {
            revealCell(board,x, y, rows, cols, diff, &score);
        } else if (command == 'f') {
            if (remainingFlags > 0) placeFlag(board, x, y, &remainingFlags);
            else printf("Uzyto wszystkie flagi, zdejmij jakas z planszy, aby postawic nowa.\n");
        } else {
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
        printf("ąąąąWitaj w grze Saper!\nWpisz liczbe, aby przejsc dalej:\n1. Zagraj.\n2. Pokaz instrukcje.\n3. Przykladowy plik.");
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
