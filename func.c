#include <stdio.h>
#include "func.h"

void display_map()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 14; j++)
        {
            printf("  ________  ");
        }
        printf("\n");
    }
}