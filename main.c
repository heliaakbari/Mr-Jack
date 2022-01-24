#include <stdio.h>
#include "func.h"
#include <conio.h>
#include <string.h>
int main()
{
    cell map[9][13];
    FILE *fp = fopen("newgame.bin", "rb");
    int a = sizeof(map[0][0].what), b = sizeof(map[0][0].watch), c = sizeof(map[0][0].who);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            fread(map[i][j].what, a, 1, fp);
            fread(map[i][j].watch, b, 1, fp);
            fread(map[i][j].who, c, 1, fp);
            fread(map[i][j].neighbor, sizeof(int), 12, fp);
        }
    }
    display_map(map);
    fclose(fp);
    getchar();
    return 0;
}