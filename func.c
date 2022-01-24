#include <stdio.h>
#include "func.h"
#include <stdlib.h>
#include <windows.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <time.h>
#define wallc 8
#define whatc 2
#define watchc 1
#define whoc 3

void SetColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    // We use csbi for the wAttributes word.
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        // Mask out all but the background attribute, and add in the forgournd color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

void display_map(cell map[9][13])
{
    SetColor(wallc);
    printf("  ");
    for (int j = 0; j < 7; j++)
    {
        printf("_______           ");
    }
    printf("\n");
    for (int j = 0; j < 14; j = j + 2)
    {
        printf(" /  ");
        SetColor(watchc);
        printf("%s", map[0][j].watch);
        SetColor(wallc);
        printf("  \\        ");
    }
    printf("\n");
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 12; j = j + 2)
        {
            printf("/");
            SetColor(whatc);
            printf("%s", map[i][j].what);
            SetColor(wallc);
            printf("\\_______");
        }
        printf("/");
        SetColor(whatc);
        printf("%s", map[i][12].what);
        SetColor(wallc);
        printf("\\");
        printf("\n");

        for (int j = 0; j < 12; j = j + 2)
        {
            printf("\\   ");
            if (!strcmp(map[i][j].who, "NU"))
            {
                SetColor(0);
            }
            else
            {
                SetColor(whoc);
            }
            printf("%s", map[i][j].who);
            SetColor(wallc);
            printf("    /  ");
            SetColor(watchc);
            printf("%s", map[i][j + 1].watch);
            SetColor(wallc);
            printf("  ");
        }
        printf("\\   ");
        if (!strcmp(map[i][12].who, "NU"))
        {
            SetColor(0);
        }
        else
        {
            SetColor(whoc);
        }
        printf("%s", map[i][12].who);
        SetColor(wallc);
        printf("    /  ");
        printf("\n");
        printf(" ");
        for (int j = 0; j < 12; j = j + 2)
        {
            printf("\\_______/");
            SetColor(whatc);
            printf("%s", map[i][j + 1].what);
            SetColor(wallc);
        }
        printf("\\_______/");
        printf("\n");
        for (int j = 0; j < 12; j = j + 2)
        {
            printf(" /  ");
            SetColor(watchc);
            printf("%s", map[i + 1][j].watch);
            SetColor(wallc);
            printf("  \\   ");
            if (!strcmp(map[i][j + 1].who, "NU"))
            {
                SetColor(0);
            }
            else
            {
                SetColor(whoc);
            }
            printf("%s", map[i][j + 1].who);
            SetColor(wallc);
            printf("   ");
        }
        printf(" /  ");
        SetColor(watchc);
        printf("%s", map[i + 1][12].watch);
        SetColor(wallc);
        printf("  \\  ");
        printf("\n");
    }
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("/");
        SetColor(whatc);
        printf("%s", map[8][j].what);
        SetColor(wallc);
        printf("\\_______");
    }
    printf("/");
    SetColor(whatc);
    printf("%s", map[8][12].what);
    SetColor(wallc);
    printf("\\");
    printf("\n");
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("\\   ");
        if (!strcmp(map[8][j].who, "NU"))
        {
            SetColor(0);
        }
        else
        {
            SetColor(whoc);
        }
        printf("%s", map[8][j].who);
        SetColor(wallc);
        printf("    /  ");
        SetColor(watchc);
        printf("%s", map[8][j + 1].watch);
        SetColor(wallc);
        printf("  ");
    }
    printf("\\  ");
    if (!strcmp(map[8][12].who, "NU"))
    {
        SetColor(0);
    }
    else
    {
        SetColor(whoc);
    }
    printf("%s", map[8][12].who);
    SetColor(wallc);
    printf("     /  ");
    printf("\n");
    printf(" ");
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("\\_______/");
        SetColor(whatc);
        printf("%s", map[8][j + 1].what);
        SetColor(wallc);
    }
    printf("\\_______/");
    printf("\n  ");
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("       \\   ");
        if (!strcmp(map[8][12].who, "NU"))
        {
            SetColor(0);
        }
        else
        {
            SetColor(whoc);
        }
        printf("%s", map[8][j + 1].who);
        SetColor(wallc);
        printf("    /");
    }
    printf("\n ");
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("         \\_______/");
    }
    printf("\n");
    return;
}

void GetDeafultGame(cell map[9][13])
{
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
    fclose(fp);
    return;
}

void create_cards(person **ptr_odd, person **ptr_even, cell map[9][13])
{
    person *list1 = (person *)malloc(sizeof(person));
    person *odd = list1;
    strncpy(odd->name, "SH", 3);
    odd->played = 0;
    odd->watch = 1;
    odd->place = &map[6][6];
    strcpy(odd->ability, "identify an innocent");
    odd->next = (person *)malloc(sizeof(person));
    odd = odd->next;
    strncpy(odd->name, "JW", 3);
    odd->played = 0;
    odd->watch = 0;
    odd->place = &map[5][0];
    strcpy(odd->ability, "iluminate 1 direction");
    odd->next = (person *)malloc(sizeof(person));
    odd = odd->next;
    strncpy(odd->name, "JS", 3);
    odd->played = 0;
    odd->watch = 1;
    odd->place = &map[3][6];
    strcpy(odd->ability, "turn off 1 lamp and turn on another");
    odd->next = (person *)malloc(sizeof(person));
    odd = odd->next;
    strncpy(odd->name, "IL", 3);
    odd->played = 0;
    odd->watch = 1;
    odd->place = &map[5][4];
    strcpy(odd->ability, "open 1 exit and close another");
    odd->next = NULL;
    *ptr_odd = list1;

    person *list2 = (person *)malloc(sizeof(person));
    odd = list2;
    strncpy(odd->name, "MS", 3);
    odd->played = 0;
    odd->watch = 0;
    odd->place = &map[8][8];
    strcpy(odd->ability, "move through apartments but can't stop in them");
    odd->next = (person *)malloc(sizeof(person));
    odd = odd->next;
    strncpy(odd->name, "SG", 3);
    odd->played = 0;
    odd->watch = 0;
    odd->place = &map[4][12];
    strcpy(odd->ability, "call chracter(s) 3 cells closer to himself");
    odd->next = (person *)malloc(sizeof(person));
    odd = odd->next;
    strncpy(odd->name, "WG", 3);
    odd->played = 0;
    odd->watch = 0;
    odd->place = &map[0][4];
    strcpy(odd->ability, "change place with another chracter");
    odd->next = (person *)malloc(sizeof(person));
    odd = odd->next;
    strncpy(odd->name, "JB", 3);
    odd->played = 0;
    odd->watch = 1;
    odd->place = &map[4][8];
    strcpy(odd->ability, "open 1 tunnel and close another");
    odd->next = NULL;
    *ptr_even = list2;
    return;
}

void shuffle(person **ptr_odd, person **ptr_even)
{
    srand(time(NULL));
    person *odd = *ptr_odd;
    person *even = *ptr_even;
    int count_odd = 0;
    int count_even = 0;
    person *ar1[4], *ar2[4], *ar[8];
    person *ptr = odd;
    for (int i = 0; i < 4; i++)
    {
        ar[i] = ptr;
        ptr = ptr->next;
    }
    ptr = even;
    for (int i = 4; i < 8; i++)
    {
        ar[i] = ptr;
        ptr = ptr->next;
    }
    int temp;
    for (int i = 0; i < 8; i++)
    {
        temp = rand() % 2;
        if ((temp == 0 && count_odd < 4) || count_even >= 4)
        {
            ar1[count_odd] = ar[i];
            count_odd++;
        }
        else if ((temp == 1 && count_even < 4) || count_odd >= 4)
        {
            ar2[count_even] = ar[i];
            count_even++;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        ar1[i]->next = ar1[i + 1];
        ar2[i]->next = ar2[i + 1];
    }

    (ar1[3])->next = NULL;
    (ar2[3])->next = NULL;
    *ptr_odd = ar1[0];
    *ptr_even = ar2[0];

    return;
}

void printfcard(person *odd, person *even)
{
    person *ptr = odd;
    while (ptr != NULL)
    {
        printf("%s ", ptr->name);
        ptr = ptr->next;
    }
    ptr = even;
    while (ptr != NULL)
    {
        printf("%s ", ptr->name);
        ptr = ptr->next;
    }
    return;
}
