#include "instruction.h"
#include <stdio.h>

void fileExample() {
    printf("9 9 1\n"
"0 0 1 0 0 0 0 0 0\n"
"0 0 0 0 0 0 0 0 0\n"
"0 0 0 0 0 0 0 0 0\n"
"0 0 0 0 0 0 0 0 0\n"
"0 0 0 0 0 0 0 0 0\n"
"0 0 0 0 0 0 0 0 0\n"
"0 0 0 0 0 0 0 0 0\n"
"0 0 0 0 0 0 0 0 0\n"
"0 0 0 0 0 0 0 0 0\n"
"r 1 1\n"
"r 2 2\n"
"r 5 5\n"
"f 1 3\n");
}

void instruction() {
    printf("\n\n=================== INSTRUKCJA ===================\n"
           "Wybierz poziom trudnosci lub wprowadz wlasne wymiary planszy i liczbe min, aby rozpoczac gre.\n"
           "\nDostepne komendy:\n"
           "  r x y  - odslania pole, gdzie x to numer rzedu, a y to numer kolumny.\n"
           "  f x y  - stawia flage, gdzie x to numer rzedu, a y to numer kolumny.\n"
           "           (Aby usunac flage, wpisz ponownie te sama komende dla tego pola.)\n"
           "\nUruchom program z parametrem -f i podaj sciezke do pliku, ktory zawiera:\n"
           "  1. Liczbe rzedow, kolumn i min (pierwsza linia).\n"
           "  2. Plansze oznaczona wartosciami:\n"
           "     0 - brak miny,\n"
           "     1 - mina.\n"
           "  3. Kolejne komendy w nowej linii.\n"
           "\nPrzykladowy format pliku (aby zobaczyc pelen plik, wpisz 3 w menu glownym):\n"
           "  9 9 10\n"
           "  0 0 1 0 0 0 0 0 0\n"
           "  0 0 0 0 0 0 0 0 0\n"
           "  ...\n"
           "  r 1 1\n"
           "  f 2 2\n"
           "\nZasady zakonczenia gry:\n"
           "  - Przegrasz, jesli odslonisz pole z mina.\n"
           "  - Wygrasz, jesli oznaczysz wszystkie miny flagami i odslonisz pozostale pola.\n"
           "\n=================================================\n\n");
}