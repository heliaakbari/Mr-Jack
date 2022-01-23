#ifndef FUNC_H
#define FUNC_H

typedef struct sell
{
    char what[10];
    char watch[4];
    char who[3];
    int neighbor[6][2];
} cell;

void display_map(cell map[9][13]);

#endif