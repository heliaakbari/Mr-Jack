#include <stdio.h>
#include "func.h"

void display_map(cell map[9][13])
{

    printf("  ");
    for (int j = 0; j < 7; j++)
    {
        printf("_______           ");
    }
    printf("\n");
    for (int j = 0; j < 14; j = j + 2)
    {
        printf(" /  ");
        printf("%s", map[0][j].watch);
        printf("  \\        ");
    }
    printf("\n");
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 12; j = j + 2)
        {
            printf("/");
            printf("%s", map[i][j].what);
            printf("\\_______");
        }
        printf("/");
        printf("%s", map[i][12].what);
        printf("\\");
        printf("\n");

        for (int j = 0; j < 12; j = j + 2)
        {
            printf("\\   ");
            printf("%s", map[i][j].who);
            printf("    /  ");
            printf("%s", map[i][j + 1].watch);
            printf("  ");
        }
        printf("\\   ");
        printf("%s", map[i][12].who);
        printf("    /  ");
        printf("\n");
        printf(" ");
        for (int j = 0; j < 12; j = j + 2)
        {
            printf("\\_______/");
            printf("%s", map[i][j + 1].what);
        }
        printf("\\_______/");
        printf("\n");
        for (int j = 0; j < 12; j = j + 2)
        {
            printf(" /  ");
            printf("%s", map[i + 1][j].watch);
            printf("  \\   ");
            printf("%s", map[i][j + 1].who);
            printf("   ");
        }
        printf(" /  ");
        printf("%s", map[i + 1][12].watch);
        printf("  \\  ");
        printf("\n");
    }
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("/");
        printf("%s", map[8][j].what);
        printf("\\_______");
    }
    printf("/");
    printf("%s", map[8][12].what);
    printf("\\");
    printf("\n");
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("\\   ");
        printf("%s", map[8][j].who);
        printf("    /  ");
        printf("%s", map[8][j + 1].watch);
        printf("  ");
    }
    printf("\\  ");
    printf("%s", map[8][12].who);
    printf("     /  ");
    printf("\n");
    printf(" ");
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("\\_______/");
        printf("%s", map[8][j + 1].what);
    }
    printf("\\_______/");
    printf("\n  ");
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("       \\   ");
        printf("%s", map[8][j + 1].who);
        printf("    /");
    }
    printf("\n ");
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("         \\_______/");
    }
    printf("\n");
}