#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp = fopen("newgame.bin", "wb");
    if (fp == NULL)
    {
        printf("file dont exist");
        return -1;
    }
    char what[10];
    char watch[4];
    char who[3];
    int x;
    int cor1, cor2;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            scanf("%d%d", &cor1, &cor2);
            scanf("%s", what);
            scanf("%s", watch);
            scanf("%s", who);
            fwrite(&cor1, sizeof(int), 1, fp);
            fwrite(&cor2, sizeof(int), 1, fp);
            fwrite(what, sizeof(char), 10, fp);
            fwrite(watch, sizeof(char), 4, fp);
            fwrite(who, sizeof(char), 3, fp);
            for (int k = 0; k < 12; k++)
            {
                scanf("%d", &x);
                fwrite(&x, sizeof(int), 1, fp);
            }
        }
    }
    fclose(fp);
    return 0;
}
