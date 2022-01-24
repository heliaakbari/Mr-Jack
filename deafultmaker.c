#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp = fopen("newgame.bin", "wb");
    if (fp == NULL)
    {
        return -1;
    }
    char what[10];
    char watch[4];
    char who[3];
    int x;
    int gar1, gar2;
    printf("hello");
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            printf("enter what(9) watch(3) who(2) for [%d][%d]\n", i, j);
            scanf("%d%d", &gar1, &gar2);
            scanf("%s", what);
            scanf("%s", watch);
            scanf("%s", who);
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
