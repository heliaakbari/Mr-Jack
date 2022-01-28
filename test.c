#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main()
{
    char c;
    FILE *fp = fopen("scoreboard.bin", "rb");
    while (!feof(fp))
    {
        c = fgetc(fp);
        printf("%c", c);
    }
    fclose(fp);
    return 0;
}
