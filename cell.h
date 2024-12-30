#ifndef CELL_H
#define CELL_H
struct cell {
    int isMine; // 0 nie ma miny, 1 jest mina
    int status; // 0 dla zakrytego, 1 dla odkrytego, 2 dla flagi
    int nearMineCount; // ile min w poblizu pola
};
#endif //CELL_H
