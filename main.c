#include <stdio.h>
#include "func.h"
#include <string.h>
#include <stdlib.h>

int main()
{
    person *odd = (person *)malloc(sizeof(person));
    person *even = (person *)malloc(sizeof(person));
    cell map[9][13];
    cell *tunnel[8] = {&map[3][0], &map[0][5], &map[3][5], &map[5][7], &map[5][12], &map[8][7], &map[1][11], &map[7][2]};
    cell *lamp[8] = {&map[6][1], &map[2][11], &map[2][2], &map[7][10], &map[3][7], &map[5][5], &map[7][7], &map[1][5]};
    MainMenu(map, &odd, &even, tunnel, lamp);
    getchar(); //
    getchar(); //
    return 0;
}