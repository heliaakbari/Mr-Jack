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
    int what;
    int watch;
    int who;
    cordinate *neighbor;
} cell;

void display_map();

#endif