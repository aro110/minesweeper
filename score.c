#include "gameWithFile.h"
#include "cell.h"
#include "instruction.h"
#include "score.h"
#include "initGame.h"
#include "printBoard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AND &&
#define OR ||
#define NOT !=

// zapisywanie wynikow do pliku
void writeScore(int *score) {
    const char *filename = "score.txt";
    FILE *f = fopen(filename, "r");

    // gracz
    typedef struct {
        char nick[100];
        int score;
    } Player;

    Player *players = NULL; // wskaźnik na tablicę graczy
    int count = 0;          // liczba graczy w rankingu
    int capacity = 0;       // maksymalna pojemność tablicy

    if (f != NULL) {
        char tempNick[100];
        int tempScore;

        while (fscanf(f, "%*d. %99[^,], wynik: %d", tempNick, &tempScore) == 2) {
            if (count == capacity) {
                capacity = (capacity == 0) ? 10 : capacity * 2;
                players = realloc(players, capacity * sizeof(Player));
                if (players == NULL) {
                    printf("Blad alokacji pamieci.\n");
                    fclose(f);
                    return;
                }
            }
            strncpy(players[count].nick, tempNick, sizeof(players[count].nick) - 1);
            players[count].nick[sizeof(players[count].nick) - 1] = '\0';
            players[count].score = tempScore;
            count++;
        }
        fclose(f);
    } else {
        printf("Blad wczytania pliku.\n");
    }

    char nick[100];
    printf("\nPodaj swoja nazwe: ");
    scanf("%99s", nick); // ograniczenie do 99 znakow

    if (count == capacity) {
        capacity = (capacity == 0) ? 10 : capacity * 2;
        players = realloc(players, capacity * sizeof(Player));
        if (players == NULL) {
            printf("Blad alokacji pamieci.\n");
            return;
        }
    }

    // dodaj nowego gracza do tablicy
    strncpy(players[count].nick, nick, sizeof(players[count].nick) - 1);
    players[count].nick[sizeof(players[count].nick) - 1] = '\0';
    players[count].score = *score;
    count++;

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
        printf("Nie mozna otworzyc pliku do zapisu.\n");
        free(players);
        return;
    }

    // zapis wszystkich graczy do pliku
    for (int i = 0; i < count; i++) {
        fprintf(f, "%d. %s, wynik: %d\n", i + 1, players[i].nick, players[i].score);
    }

    fclose(f);

    // wyświetlenie 5 najlepszych graczy
    printf("\nAktualny ranking (top 5):\n");
    for (int i = 0; i < (count < 5 ? count : 5); i++) { // pokazuje tylko 5 najlepszych
        printf("%d. %s, wynik: %d\n", i + 1, players[i].nick, players[i].score);
    }

    free(players);
}

// ustawienie poziomu trudnosci
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

// sprawdzanie wygranej
int checkWin(struct cell **board, int rows, int cols) {
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            // Sprawdza, czy wszystkie miny są oznaczone flagami
            if (board[x][y].isMine AND board[x][y].status NOT 2) return 0;
            // Sprawdza, czy wszystkie pola bez min są odkryte
            if (!board[x][y].isMine AND board[x][y].status NOT 1) return 0;
        }
    }
    return 1; // Wszystkie warunki wygranej spełnione
}

