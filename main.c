#include <stdio.h>
#include "func.h"
#include <conio.h>
#include <string.h>
int main()
{
    cell arr[9][13];
    // display_map(arr);

    FILE *fp = fopen("newgame.bin", "rb");
    fread(arr[0][0].what, sizeof(arr[0][0].what), 1, fp);
    fread(arr[0][0].watch, sizeof(arr[0][0].watch), 1, fp);
    fread(arr[0][0].who, sizeof(arr[0][0].who), 1, fp);
    printf("%s%s%s", arr[0][0].what, arr[0][0].watch, arr[0][0].who);
    printf("%d %d %d", strlen(arr[0][0].what), strlen(arr[0][0].watch), strlen(arr[0][0].who));
    fclose(fp);
    getchar();
    return 0;
}