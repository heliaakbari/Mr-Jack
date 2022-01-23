#ifndef FUNC_H
#define FUNC_H
typedef struct cor
{
    int x;
    int y;
    struct cor *next;
} cordinate;

typedef struct sell
{
    char what[10];
    char watch[4];
    char who[3];
    cordinate *neighbor;
} cell;

void display_map(cell map[9][13]);

#endif