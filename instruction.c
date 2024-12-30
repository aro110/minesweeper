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
    printf("\n\nWybierz poziom trudnosci, badz wprowadz swoje wymiary planszy i liczbe min, aby rozpoczac gre!\n"
           "Komendy, potrzebne do gry:\n"
           "r x y - odslania pole, gdzie x to numer rzedu, a y to numer kolumny,\n"
           "f x y - stawia flage, gdzie x to numer rzedu, a y to numer kolumny (aby usunac flage wpisz tez te komende ze wspolrzednymi flagi).\n"
           "Uruchom program z parametrem -f i podaj sciezke do pliku, ktory zawiera gotowa plansze i komendy, aby sprawdzic\npoprawnosc liczby ruchow, ilosc zdobytych punktowo oraz czy wygrales!\n"
           "W pliku na poczatku podaj 3 liczby, ktore znacza kolejno liczbe rzedow i kolumn planszy oraz liczbe min,\nktora ustawilesw planszy.\n"
           "Ponizej umiesc plansze o tych wymiarach ktore podales i oznacz pola liczbami 0 - brak miny; 1 - mina.\n"
           "Nastepnie, za kazdym razem od nowej linii podaj komende, ktora uzyles.\n"
           "Przykladowy plik w poprawnej formie znajdziesz w menu glownym.\n"
           "Gra sie konczy w momencie trafienia na mine, badz oznaczenia flaga kazdej miny na planszy.\n\n");
}