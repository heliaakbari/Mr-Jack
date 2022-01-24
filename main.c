#include <stdio.h>
#include "func.h"
#include <conio.h>
#include <string.h>
cell map[9][13];

int main()
{
    GetDeafultGame(map);
    display_map(map);
    person *odd, *even;
    create_cards(&odd, &even, map);
    for (int i = 0; i < 5; i++)
    {
        shuffle(&odd, &even);
        printfcard(odd, even);
        printf("\n");
    }
    getchar();
    return 0;
}