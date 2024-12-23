#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
 */

#define AND &&
#define OR ||
#define NOT !=

#define PAUSE system("pause")
#define CLEAR system("cls")

struct cell {
    int isMine; // 0 nie ma miny, 1 jest mina
    int status; // 0 dla zakrytego, 1 dla odkrytego, 2 dla flagi
    int nearMineCount; // ile min w poblizu pola
};

void placeFlag(struct cell **board, int x, int y) {
    if (board[x][y].status == 0) {
        board[x][y].status = 2;
    }
    else if (board[x][y].status == 2) {
        board[x][y].status = 0;
    }
}

void revealCell (struct cell **board, int x, int y, int rows, int cols) {
    if (x<0 OR y<0 OR x>=rows OR y>=cols OR board[x][y].status == 1 OR board[x][y].status == 2) return;

    board[x][y].status = 1;

    if (board[x][y].isMine) {
        printf("Koniec gry, mina!\n");
        exit(0);
    }

    if(board[x][y].nearMineCount == 0) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx NOT 0 OR dy NOT 0) {
                    revealCell(board, x+dx, y+dy, rows, cols);
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
            printf("Mina na: %d %d\n", x, y);
        }
    }
}

void printBoard(struct cell **board, int rows, int cols) {
    // naglowki kolumn
    printf("    ");
    for (int i = 0; i < cols; i++) {
        printf(" %2d ", i+1);
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
                    printf(" %d |", board[x][y].nearMineCount);
                }
            } else if (board[x][y].status == 2) { // flaga
                printf(" P |");
            } else { // Pole zakryte
                printf("   |");
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


void initBoard(struct cell **board, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j].isMine = 0;
            board[i][j].status = 0;
            board[i][j].nearMineCount = 0;
        }
    }
}

int diffLevel() {
    int diff;
    printf("Wybierz poziom trudnosci:\n1. Latwy\n2. Sredni\n3. Trudny");
    scanf("%d", &diff);
    if (diff == 1) {
        return 0;
    } else if (diff == 2) {
        return 1;
    } else if (diff == 3) {
        return 2;
    } else {
        printf("Blad wprowadzania danych");
        PAUSE;
    }
}

int checkWin(struct cell **board, int rows, int cols) {
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            if (board[x][y].isMine AND board[x][y].status NOT 2) return 0;
        }
    }
    return 1;
}

void instruction() {
    printf("instrukcja");
}

void minesweeper() {
    int userInputX;
    int userInputY;
    int firstInput = 1;
    int rows;
    int cols;
    char command;
    int numMin;

    int diff = diffLevel(); // 0 latwy, 1 sredni, 2 trudny
    switch (diff) {
        case 0:
            rows = 9;
            cols = 9;
            numMin = 10;
            break;
        case 1:
            rows = 16;
            cols = 16;
            numMin = 40;
            break;
        case 2:
            rows = 16;
            cols = 30;
            numMin = 99;
            break;
    }

    struct cell **board = malloc(sizeof(struct cell *) * rows);
    for (int i = 0; i < rows; i++) {
        board[i] = malloc(sizeof(struct cell) * cols);
    }

    initBoard(board, rows, cols);
    while (1) {
        printBoard(board, rows, cols);

        printf("Wprowadz wspolrzedne do odsloniecia (r/f x y): ");
        if (scanf(" %c %d %d", &command, &userInputX, &userInputY) NOT 3) {
            printf("Blad wprowadzania danych. Sprobuj ponownie.\n");
            while (getchar() NOT '\n'); // Czyszczenie bufora wejścia
            continue;
        }

        int x = userInputX - 1;
        int y = userInputY - 1;

        if (firstInput) {
            placeMines(board, rows, cols, numMin, x, y);
            nearMines(board, rows, cols);
            firstInput = 0;
        }

        if (command == 'r') {
            revealCell(board,x, y, rows, cols);
        } else if (command == 'f') {
            placeFlag(board, x, y);
        } else {
            printf("Blad wprowadzania danych\n");
        }

        if (checkWin(board, rows, cols)) {
            printf("Koniec gry, wygrales!");

            exit(0);
        }
    }
}

int main(void) {
    srand(time(NULL));

    int choice;

    printf("Witaj w grze Saper!\nWpisz liczbe, aby przejsc dalej:\n1. Zagraj.\n2. Pokaz instrukcje.");
    scanf("%d", &choice);
    if (choice == 1) {
        minesweeper();
    } else if (choice == 2) {
        instruction();
    } else {
        printf("Blad wprowadzania danych.");
    }

    PAUSE;
    return 0;
}
