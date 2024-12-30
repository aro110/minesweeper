#include "gameWithFile.h"
#include "cell.h"
#include "instruction.h"
#include "score.h"
#include "initGame.h"
#include "printBoard.h"
#include <stdio.h>
#include <string.h>

#define AND &&
#define OR ||
#define NOT !=

void writeScore(int *score) {
    const char *filename = "score.txt";
    FILE *f = fopen(filename, "r");

    // gracz
    typedef struct {
        char nick[100];
        int score;
    } Player;

    Player players[5];
    int count = 0; // liczba graczy w rankingu

    if (f != NULL) {
        while (fscanf(f, "%*d. %99[^,], wynik: %d", players[count].nick, &players[count].score) == 2) {
            count++;
            if (count == 5) break;
        }
        fclose(f);
    } else {
        printf("Blad wczytania pliku\n");
    }

    char nick[100];
    printf("\nPodaj swoja nazwe: ");
    scanf("%99s", nick); // ograniczenie do 99 znakow

    // dodaj nowego gracza do tablicy na ostatnie miejsce
    if (count < 5 || players[count - 1].score < *score) {
        if (count < 5) {
            count++;
        }
        players[count - 1].score = *score;
        strncpy(players[count - 1].nick, nick, sizeof(players[count - 1].nick) - 1); // kopiowanie nazwy do 99 znakow, tak aby ostatni byl NULL
        players[count - 1].nick[sizeof(players[count - 1].nick) - 1] = '\0';
    } else {
        printf("Nie dostales sie do najlepszej piatki. Sprobuj nastepnym razem!\n");
    }

    // sortowanie malejaco wedlug wyniku
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (players[j].score < players[j + 1].score) {
                Player temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }

    f = fopen(filename, "w");
    if (f == NULL) {
        printf("Nie mozna otworzyc pliku do zapisu\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(f, "%d. %s, wynik: %d\n", i + 1, players[i].nick, players[i].score);
    }

    fclose(f);

    // ranking
    printf("\nAktualny ranking:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s, wynik: %d\n", i + 1, players[i].nick, players[i].score);
    }
}

int diffLevel() {
    int diff;
    printf("Wybierz poziom trudnosci:\n1. Latwy\n2. Sredni\n3. Trudny\n4. Wlasna plansza");
    scanf("%d", &diff);
    if (diff == 1) {
        return 1;
    } else if (diff == 2) {
        return 2;
    } else if (diff == 3) {
        return 3;
    } else if (diff == 4) {
        return 4;
    } else {
        printf("Blad wprowadzania danych");
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
