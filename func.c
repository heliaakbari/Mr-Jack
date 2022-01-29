#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dos.h>
#include <dir.h>
#include <ctype.h>
#include <string.h>
#include "func.h"
#include <time.h>
#include <conio.h>
int jackvisibility = 1;
char jackname[20];
char decname[20];
int turnglobal = 1;
int endglobal = 0;
int roundglobal = 1;
person *jackglobal;
int johndirection = 3;
person *odd;
person *even;
cell map[9][13];
cell *out[] = {&map[0][1], &map[0][11], &map[8][1], &map[8][11]};
cell *tunnel[] = {&map[3][0], &map[0][5], &map[3][5], &map[5][7], &map[5][12], &map[8][7], &map[1][11], &map[7][2]};
cell *lamp[] = {&map[6][1], &map[2][11], &map[2][2], &map[7][10], &map[3][7], &map[5][5], &map[7][7], &map[1][5]};

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

void paint(char *str)
{
    enum color
    {
        black,
        blue,
        green,
        cyan,
        red,
        magenta,
        brown,
        lightgray,
        darkgray,
        lightblue,
        lightgreen,
        lightcyan,
        lightred,
        lightmagenta,
        yellow
    };
    if (!strncmp(str, "apartment", 4))
        SetColor(blue);
    else if (!strncmp(str, "lamp", 4))
        SetColor(lightmagenta);
    else if (!strncmp(str, "side", 4))
        SetColor(black);
    else if (!strncmp(&str[5], "clsd", 3) && !strncmp(str, "tnl", 3))
        SetColor(red);
    else if (!strncmp(&str[5], "open", 3) && !strncmp(str, "tnl", 3))
        SetColor(lightred);
    else if (!strncmp(str, "on!", 3))
    {

        SetColor(black);
        printf("|");
        SetColor(blue);
        printf("1");
        SetColor(black);
        printf("|");
        SetColor(blue);
        return;
    }
    else if (!strncmp(str, "off", 3))
    {
        SetColor(black);
        printf("|");
        SetColor(black);
        printf("0");
        SetColor(black);
        printf("|");
        SetColor(blue);
        return;
    }
    else if (!strncmp(str, "out", 3))
        SetColor(lightcyan);
    else if (!strncmp(str, "NU", 2))
        SetColor(black);
    else if (strlen(str) == 2)
    {
        SetColor(lightgreen);
    }
    printf("%s", str);
    SetColor(blue);
}

void display_map()
{
    SetColor(1);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 12; j = j + 2)
        {
            printf("/");
            paint(map[i][j].what);
            printf("\\_______");
        }
        printf("/");
        paint(map[i][12].what);
        printf("\\");
        printf("\n");

        for (int j = 0; j < 12; j = j + 2)
        {
            printf("\\   ");
            paint(map[i][j].who);
            printf("    /  ");
            paint(map[i][j + 1].watch);
            printf("  ");
        }
        printf("\\   ");
        paint(map[i][12].who);
        printf("    /  ");
        printf("\n");
        printf(" ");
        for (int j = 0; j < 12; j = j + 2)
        {
            printf("\\_______/");
            paint(map[i][j + 1].what);
        }
        printf("\\_______/");
        printf("\n");
        for (int j = 0; j < 12; j = j + 2)
        {
            printf(" /  ");
            paint(map[i + 1][j].watch);
            printf("  \\   ");
            paint(map[i][j + 1].who);
            printf("   ");
        }
        printf(" /  ");
        paint(map[i + 1][12].watch);
        printf("  \\  ");
        printf("\n");
    }
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("/");
        paint(map[8][j].what);
        printf("\\_______");
    }
    printf("/");
    paint(map[8][12].what);
    printf("\\");
    SetColor(15);
    if ((turnglobal % 4 == 1 && roundglobal % 2 == 1) || (turnglobal % 4 == 0 && roundglobal % 2 == 1) || (turnglobal % 4 == 2 && roundglobal % 2 == 0) || (turnglobal % 4 == 3 && roundglobal % 2 == 0))
    {
        printf("    Detective's turn(round%d,turn%d)", roundglobal, turnglobal);
    }
    else
    {
        printf("    Jacks's turn(round%d,turn%d)", roundglobal, turnglobal);
    }
    printf("\n");
    SetColor(1);
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("\\   ");

        paint(map[8][j].who);
        printf("    /  ");
        paint(map[8][j + 1].watch);
        printf("  ");
    }
    printf("\\  ");
    paint(map[8][12].who);
    printf("     /  ");
    SetColor(15);
    if (jackvisibility == 0)
    {
        printf("  Jack is invisible");
    }
    else
    {
        printf("  Jack is visible");
    }
    SetColor(1);
    printf("\n");
    printf(" ");
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("\\_______/");
        paint(map[8][j + 1].what);
    }
    printf("\\_______/");
    SetColor(15);
    printf("     suspects: ");
    person *ptr = odd;
    while (ptr != NULL)
    {
        if (ptr->guilty == 0)
        {
            printf("%s ", ptr->name);
        }
        ptr = ptr->next;
    }
    ptr = even;
    while (ptr != NULL)
    {
        if (ptr->guilty == 0)
        {
            printf("%s ", ptr->name);
        }
        ptr = ptr->next;
    }
    SetColor(1);
    printf("\n  ");
    for (int j = 0; j < 12; j = j + 2)
    {
        printf("       \\___");

        paint(map[8][j + 1].who);
        printf("____/");
    }
    SetColor(15);
    printf("             innocents: ");
    ptr = odd;
    while (ptr != NULL)
    {
        if (ptr->guilty == 1)
        {
            printf("%s ", ptr->name);
        }
        ptr = ptr->next;
    }
    ptr = even;
    while (ptr != NULL)
    {
        if (ptr->guilty == 1)
        {
            printf("%s ", ptr->name);
        }
        ptr = ptr->next;
    }
    printf("\n ");
    return;
}

void savethegame()
{
    FILE *fp = fopen("savedgame.bin", "wb");
    if (fp == NULL)
    {
        printf("no file found ");
        MainMenu();
    }
    int n = -1;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            fwrite(map[i][j].what, sizeof(char), 10, fp);
            fwrite(map[i][j].watch, sizeof(char), 4, fp);
            fwrite(map[i][j].who, sizeof(char), 3, fp);
            for (int k = 0; k < 6; k++)
            {
                if (map[i][j].neighbor[k] == NULL)
                {
                    fwrite(&n, sizeof(int), 1, fp);
                    fwrite(&n, sizeof(int), 1, fp);
                }
                else
                {
                    fwrite(&(map[i][j].neighbor[k]->x), sizeof(int), 1, fp);
                    fwrite(&(map[i][j].neighbor[k]->y), sizeof(int), 1, fp);
                }
            }
        }
    }
    fwrite(&jackvisibility, sizeof(int), 1, fp);
    fwrite(&turnglobal, sizeof(int), 1, fp);
    fwrite(&endglobal, sizeof(int), 1, fp);
    fwrite(&roundglobal, sizeof(int), 1, fp);
    fwrite(&(jackglobal->place->x), sizeof(int), 1, fp);
    fwrite(&(jackglobal->place->y), sizeof(int), 1, fp);
    fwrite(&johndirection, sizeof(int), 1, fp);
    person *ptr = odd;
    while (ptr != NULL)
    {
        fwrite(ptr->name, sizeof(char), 3, fp);
        fwrite((&ptr->played), sizeof(int), 1, fp);
        fwrite(&(ptr->guilty), sizeof(int), 1, fp);
        fwrite(&(ptr->watch), sizeof(int), 1, fp);
        fwrite(&(ptr->got), sizeof(int), 1, fp);
        fwrite(&(ptr->place->x), sizeof(int), 1, fp);
        fwrite(&(ptr->place->y), sizeof(int), 1, fp);
        ptr = ptr->next;
    }
    ptr = even;
    while (ptr != NULL)
    {
        fwrite(ptr->name, sizeof(char), 3, fp);
        fwrite((&ptr->played), sizeof(int), 1, fp);
        fwrite(&(ptr->guilty), sizeof(int), 1, fp);
        fwrite(&(ptr->watch), sizeof(int), 1, fp);
        fwrite(&(ptr->got), sizeof(int), 1, fp);
        fwrite(&(ptr->place->x), sizeof(int), 1, fp);
        fwrite(&(ptr->place->y), sizeof(int), 1, fp);
        ptr = ptr->next;
    }
    fwrite(jackname, sizeof(char), 20, fp);
    fwrite(decname, sizeof(char), 20, fp);
    fclose(fp);
    return;
}

void resumethegame()
{
    FILE *fp = fopen("savedgame.bin", "rb");
    if (fp == NULL)
    {
        printf("no saved game found, please start a new game");
        MainMenu();
    }

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            map[i][j].x = i;
            map[i][j].y = j;
            fread(map[i][j].what, sizeof(char), 10, fp);
            fread(map[i][j].watch, sizeof(char), 4, fp);
            fread(map[i][j].who, sizeof(char), 3, fp);
            for (int k = 0; k < 6; k++)
            {
                int t1, t2;
                fread(&t1, sizeof(int), 1, fp);
                fread(&t2, sizeof(int), 1, fp);
                if (t1 == -1 && t2 == -1)
                {
                    map[i][j].neighbor[k] = NULL;
                }
                else
                {
                    map[i][j].neighbor[k] = &map[t1][t2];
                }
            }
        }
    }
    fread(&jackvisibility, sizeof(int), 1, fp);
    fread(&turnglobal, sizeof(int), 1, fp);
    fread(&endglobal, sizeof(int), 1, fp);
    fread(&roundglobal, sizeof(int), 1, fp);
    int t1, t2;
    fread(&t1, sizeof(int), 1, fp);
    fread(&t2, sizeof(int), 1, fp);
    fread(&johndirection, sizeof(int), 1, fp);
    create_cards();
    person *ptr = odd;
    while (ptr != NULL)
    {
        fread(ptr->name, sizeof(char), 3, fp);
        fread((&ptr->played), sizeof(int), 1, fp);
        fread(&(ptr->guilty), sizeof(int), 1, fp);
        fread(&(ptr->watch), sizeof(int), 1, fp);
        fread(&(ptr->got), sizeof(int), 1, fp);
        int t1, t2;
        fread(&t1, sizeof(int), 1, fp);
        fread(&t2, sizeof(int), 1, fp);
        ptr->place = &map[t1][t2];
        ptr = ptr->next;
    }
    ptr = even;
    while (ptr != NULL)
    {
        fread(ptr->name, sizeof(char), 3, fp);
        fread((&ptr->played), sizeof(int), 1, fp);
        fread(&(ptr->guilty), sizeof(int), 1, fp);
        fread(&(ptr->watch), sizeof(int), 1, fp);
        fread(&(ptr->got), sizeof(int), 1, fp);
        int t1, t2;
        fread(&t1, sizeof(int), 1, fp);
        fread(&t2, sizeof(int), 1, fp);
        ptr->place = &map[t1][t2];
        ptr = ptr->next;
    }
    jackglobal = FindThePerson(map[t1][t2].who);
    fread(jackname, sizeof(char), 20, fp);
    fread(decname, sizeof(char), 20, fp);
    fclose(fp);
    return;
}

void GetDeafultGame()
{
    FILE *fp = fopen("newgame.bin", "rb");
    int a = sizeof(map[0][0].what), b = sizeof(map[0][0].watch), c = sizeof(map[0][0].who);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            fread(&map[i][j].x, sizeof(int), 1, fp);
            fread(&map[i][j].y, sizeof(int), 1, fp);
            fread(map[i][j].what, a, 1, fp);
            fread(map[i][j].watch, b, 1, fp);
            fread(map[i][j].who, c, 1, fp);
            for (int k = 0; k < 6; k++)
            {
                int t1, t2;
                fread(&t1, sizeof(int), 1, fp);
                fread(&t2, sizeof(int), 1, fp);
                if (t1 == -1 && t2 == -1)
                {
                    map[i][j].neighbor[k] = NULL;
                }
                else
                {
                    map[i][j].neighbor[k] = &map[t1][t2];
                }
            }
        }
    }
    strcpy(map[8][0].watch, "off");
    fclose(fp);
    return;
}

void create_cards()
{
    odd = (person *)malloc(sizeof(person));
    person *tptr = odd;
    strncpy(tptr->name, "SH", 3);
    tptr->played = 0;
    tptr->got = 0;
    tptr->watch = 1;
    tptr->guilty = 0;
    tptr->place = &map[6][6];
    tptr->next = (person *)malloc(sizeof(person));
    tptr = tptr->next;
    strncpy(tptr->name, "JW", 3);
    tptr->played = 0;
    tptr->got = 0;
    tptr->guilty = 0;
    tptr->watch = 0;
    tptr->place = &map[5][0];
    tptr->next = (person *)malloc(sizeof(person));
    tptr = tptr->next;
    strncpy(tptr->name, "JS", 3);
    tptr->played = 0;
    tptr->got = 0;
    tptr->guilty = 0;
    tptr->watch = 1;
    tptr->place = &map[3][6];
    tptr->next = (person *)malloc(sizeof(person));
    tptr = tptr->next;
    strncpy(tptr->name, "IL", 3);
    tptr->played = 0;
    tptr->got = 0;
    tptr->guilty = 0;
    tptr->watch = 1;
    tptr->place = &map[5][4];
    tptr->next = NULL;
    even = (person *)malloc(sizeof(person));
    tptr = even;
    strncpy(tptr->name, "MS", 3);
    tptr->played = 0;
    tptr->got = 0;
    tptr->guilty = 0;
    tptr->watch = 0;
    tptr->place = &map[8][8];
    tptr->next = (person *)malloc(sizeof(person));
    tptr = tptr->next;
    strncpy(tptr->name, "SG", 3);
    tptr->played = 0;
    tptr->watch = 0;
    tptr->guilty = 0;
    tptr->got = 0;
    tptr->place = &map[4][12];
    tptr->next = (person *)malloc(sizeof(person));
    tptr = tptr->next;
    strncpy(tptr->name, "WG", 3);
    tptr->played = 0;
    tptr->guilty = 0;
    tptr->got = 0;
    tptr->watch = 0;
    tptr->place = &map[1][4];
    tptr->next = (person *)malloc(sizeof(person));
    tptr = tptr->next;
    strncpy(tptr->name, "JB", 3);
    tptr->played = 0;
    tptr->guilty = 0;
    tptr->got = 0;
    tptr->watch = 1;
    tptr->place = &map[4][8];
    tptr->next = NULL;
    return;
}

void shuffle()
{
    // srand(time(NULL));
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
    even = ar2[0];
    odd = ar1[0];
    return;
}

void printfcard()
{
    person *ptr = odd;
    while (ptr != NULL)
    {
        printf("%s ,", ptr->name);
        printf("%s ,", ptr->place->what);
        ptr = ptr->next;
    }
    ptr = even;
    while (ptr != NULL)
    {
        printf("%s ", ptr->name);
        printf("%s ,", ptr->place->what);
        ptr = ptr->next;
    }
    printf("\n");
    return;
}

person *FindThePerson(char names[3])
{
    person *ptr = odd;
    while (ptr != NULL)
    {
        if (!strcmp(ptr->name, names))
        {
            return ptr;
        }
        ptr = ptr->next;
    }
    ptr = even;
    while (ptr != NULL)
    {
        if (!strcmp(ptr->name, names))
        {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

char *GetTheMove()
{
    char c1;
    char *des = (char *)malloc(10 * sizeof(char));
    for (int i = 0; i < 10; i++)
    {
        des[i] = '\0';
    }
    char *ptr = des;
    c1 = getchar();
    while (isprint((int)(c1)) || strlen(des) == 0)
    {

        if (c1 != ' ')
        {
            *ptr = c1;
            ptr = ptr + 1;
        }
        c1 = getchar();
    }
    return des;
}

int CanMove(person mover, char *move, cell **last)
{
    char *ptr = move;
    while (*ptr == '\n')
    {
        ptr = ptr + 1;
    }
    int count = 0;
    cell *where = mover.place;
    int total = 3;
    if (!strcmp(mover.name, "MS"))
    {
        total = 4;
    }
    while (*ptr != '\0' && ptr != "\n")
    {
        if (*ptr == 'u')
        {
            where = where->neighbor[0];
            if (where == NULL)
            {
                return -1;
            }
            else if (!strncmp(where->what, "apartment", 3) || !strncmp(where->what, "lamp", 3))
            {
                if (!strcmp(mover.name, "MS"))
                {
                    ;
                }
                else
                {
                    return -2;
                }
            }
        }
        else if (*ptr == 'i')
        {
            where = where->neighbor[1];
            if (where == NULL)
            {
                return -3;
            }
            else if (!strncmp(where->what, "apartment", 3) || !strncmp(where->what, "lamp", 3))
            {
                if (!strcmp(mover.name, "MS"))
                {
                    ;
                }
                else
                {
                    return -4;
                }
            }
        }
        else if (*ptr == 'k')
        {
            where = where->neighbor[2];
            if (where == NULL)
            {
                return -5;
            }
            else if (!strncmp(where->what, "apartment", 3) || !strncmp(where->what, "lamp", 3))
            {
                if (!strcmp(mover.name, "MS"))
                {
                    ;
                }
                else
                {
                    return -6;
                }
            }
        }
        else if (*ptr == 'm')
        {
            where = where->neighbor[3];
            if (where == NULL)
            {
                return -7;
            }
            else if (!strncmp(where->what, "apartment", 3) || !strncmp(where->what, "lamp", 3))
            {
                if (!strcmp(mover.name, "MS"))
                {
                    ;
                }
                else
                {
                    return -8;
                }
            }
        }
        else if (*ptr == 'n')
        {
            where = where->neighbor[4];
            if (where == NULL)
            {
                return -9;
            }
            else if (!strncmp(where->what, "apartment", 3) || !strncmp(where->what, "lamp", 3))
            {
                if (!strcmp(mover.name, "MS"))
                {
                    ;
                }
                else
                {
                    return -10;
                }
            }
        }
        else if (*ptr == 'h')
        {
            where = where->neighbor[5];
            if (where == NULL)
            {
                return -11;
            }
            else if (!strncmp(where->what, "apartment", 3) || !strncmp(where->what, "lamp", 3))
            {
                if (!strcmp(mover.name, "MS"))
                {
                    ;
                }
                else
                {
                    return -12;
                }
            }
        }
        else if (*ptr == 'j')
        {
            if (strncmp(&(where->what[5]), "open", 3))
            {
                return -13;
            }
            count--;
        }
        else if (*ptr >= '1' && *ptr <= '8')
        {
            int num = (int)(*ptr) - 49;
            where = tunnel[num];
            if (strncmp(&(where->what[5]), "open", 3))
            {
                return -14;
            }
        }

        count++;
        ptr = ptr + 1;
        if (count > total)
        {
            return -15;
        }
    }
    if (!strncmp(where->what, "apartment", 4) || !strncmp(where->what, "lamp", 3))
    {

        return -16;
    }
    if (strcmp(where->who, "NU"))
    {
        if ((turnglobal % 4 == 1 && roundglobal % 2 == 1) || (turnglobal % 4 == 0 && roundglobal % 2 == 1) || (turnglobal % 4 == 2 && roundglobal % 2 == 0) || (turnglobal % 4 == 3 && roundglobal % 2 == 0))
        {
            if (!strncmp(where->who, jackglobal->place->who, 2))
            {
                win();
            }
            else
            {
                loose();
            }
        }
        else
        {
            return -17;
        }
    }
    if (jackvisibility == 0 && !strcmp(mover.name, jackglobal->name))
    {
        for (int i = 0; i < 4; i++)
        {
            if ((*last) == tunnel[i])
            {
                loose();
                exit(0);
            }
        }
    }
    if (*last == mover.place)
    {
        return -18;
    }
    *last = where;

    return 1;
}

void newturn()
{
    system("cls");
    display_map();
    printf("choose one(write the initials-capital letter-and press enter):   ");
    if (roundglobal % 2 == 1)
    {
        person *ptr = odd;
        while (ptr != NULL)
        {
            if (ptr->played == 0)
            {
                printf("%s ", ptr->name);
            }
            ptr = ptr->next;
        }
    }
    else if (roundglobal % 2 == 0)
    {
        person *ptr = even;
        while (ptr != NULL)
        {
            if (ptr->played == 0)
            {
                printf("%s ", ptr->name);
            }
            ptr = ptr->next;
        }
    }
    printf("   :   ");
    char chosen[3];
    scanf(" %s", chosen);
    if (!strncmp(chosen, "SH", 2))
    {
        SH_play();
    }
    else if (!strncmp(chosen, "JW", 2))
    {
        JW_play();
    }
    else if (!strncmp(chosen, "JS", 2))
    {
        JS_play();
    }
    else if (!strncmp(chosen, "IL", 2))
    {
        IL_play();
    }
    else if (!strncmp(chosen, "MS", 2))
    {
        MS_play();
    }
    else if (!strncmp(chosen, "SG", 2))
    {
        SG_play();
    }
    else if (!strncmp(chosen, "WG", 2))
    {
        WG_play();
    }
    else if (!strncmp(chosen, "JB", 2))
    {
        JB_play();
    }
    else
    {
        newturn();
    }

    return;
}

void NewRound()
{
    person *ptr = odd;
    while (ptr != NULL)
    {
        ptr->played = 0;
        ptr = ptr->next;
    }
    ptr = even;
    while (ptr != NULL)
    {
        ptr->played = 0;
        ptr = ptr->next;
    }
    if (roundglobal <= 5 && roundglobal >= 2)
    {
        int i = 0;
        for (i = 0; i < 8; i++)
        {
            if (!strncmp(&(lamp[i]->what[6]), "on!", 3))
                break;
        }
        if (i == 0)
        {
            strcpy(lamp[i]->what, "lamp1-off");
        }
        else if (i == 1)
        {
            strcpy(lamp[i]->what, "lamp2-off");
        }
        else if (i == 2)
        {
            strcpy(lamp[i]->what, "lamp3-off");
        }
        else if (i == 3)
        {
            strcpy(lamp[i]->what, "lamp4-off");
        }
        else if (i == 4)
        {
            strcpy(lamp[i]->what, "lamp5-off");
        }
        else if (i == 5)
        {
            strcpy(lamp[i]->what, "lamp6-off");
        }
        else if (i == 6)
        {
            strcpy(lamp[i]->what, "lamp7-off");
        }
        else if (i == 7)
        {
            strcpy(lamp[i]->what, "lamp8-off");
        }
        for (int j = 0; j < 6; j++)
        {
            if (lamp[i]->neighbor[j] == NULL)
            {
                continue;
            }
            strcpy(lamp[i]->neighbor[j]->watch, "off");
        }
        johnlight(FindThePerson("JW")->place, johndirection);
    }
    turnglobal = 1;
    if (roundglobal % 2 == 1)
    {
        shuffle();
    }
    for (int i = 0; i < 4; i++)
    {
        newturn();
        turnglobal++;
        checkend();
        savethegame();
    }
    roundglobal++;
    if (roundglobal == 9)
    {
        loose();
    }
    checkvisibility();
    NewRound();
}

void checkvisibility()
{
    if (!strcmp(jackglobal->place->watch, "on!"))
    {
        jackvisibility = 1;
        jackglobal->watch = 1;
    }
    else if (!strcmp(jackglobal->place->watch, "off"))
    {
        jackvisibility = 0;
        jackglobal->watch = 0;
    }
    for (int i = 0; i < 6; i++)
    {
        if (jackglobal->place->neighbor[i] == NULL)
        {
            continue;
        }
        if (strcmp(jackglobal->place->neighbor[i]->who, "NU"))
        {
            jackvisibility = 1;
            jackglobal->watch = 1;
        }
    }
    person *ptr = odd;
    while (ptr != NULL)
    {
        if (!strcmp(ptr->place->watch, "on!"))
        {
            ptr->watch = 1;
        }
        else if (!strcmp(ptr->place->watch, "off"))
        {
            ptr->watch = 0;
        }
        for (int i = 0; i < 6; i++)
        {
            if (ptr->place->neighbor[i] == NULL)
            {
                continue;
            }
            if (strcmp(ptr->place->neighbor[i]->who, "NU"))
            {
                ptr->watch = 1;
            }
        }
        ptr = ptr->next;
    }
    ptr = even;
    while (ptr != NULL)
    {
        if (!strcmp(ptr->place->watch, "on!"))
        {
            ptr->watch = 1;
        }

        else if (!strcmp(ptr->place->watch, "off"))
        {
            ptr->watch = 0;
        }
        for (int i = 0; i < 6; i++)
        {
            if (ptr->place->neighbor[i] == NULL)
            {
                continue;
            }
            if (strcmp(ptr->place->neighbor[i]->who, "NU"))
            {
                ptr->watch = 1;
            }
        }
        ptr = ptr->next;
    }
    ptr = odd;
    while (ptr != NULL)
    {
        if (ptr->watch != jackglobal->watch)
        {
            ptr->guilty = 1;
        }
        ptr = ptr->next;
    }
    ptr = even;
    while (ptr != NULL)
    {
        if (ptr->watch != jackglobal->watch)
        {
            ptr->guilty = 1;
        }
        ptr = ptr->next;
    }
    return;
}

void checkend()
{
    if (jackvisibility == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            if (!strcmp(out[i]->who, jackglobal->name))
            {
                loose();
            }
        }
    }

    return;
}

void johnlight(cell *place, int direction)
{
    if (place->neighbor[direction] == NULL || !strcmp(place->neighbor[direction]->what, "apartment"))
    {
        return;
    }
    cell *ptr = place->neighbor[direction];
    while (ptr->neighbor[direction] != NULL && strcmp(ptr->neighbor[direction]->what, "apartment"))
    {
        strcpy(ptr->watch, "on!");
        ptr = ptr->neighbor[direction];
    }
    strcpy(ptr->watch, "on!");
    for (int i = 0; i < 6; i++)

        return;
}

void johnlight_off(cell *place, int direction)
{

    cell *ptr = place->neighbor[direction];
    while (ptr->neighbor[direction] != NULL)
    {
        strcpy(ptr->watch, "off");
        ptr = ptr->neighbor[direction];
    }
    strcpy(ptr->watch, "off");
    for (int i = 0; i < 6; i++)
    {
        if (!strncmp(&(lamp[i]->what[6]), "on!", 3))
        {
            if (lamp[0]->neighbor[i] == NULL)
            {
                continue;
            }
            strcpy(lamp[0]->neighbor[i]->watch, "on!");
        }
    }
    return;
}

void SH_play()
{
    printf("SH : move 1 to 3 cells : ");
    cell *last;
    person *sherlock = FindThePerson("SH");
    char *m = GetTheMove();
    int can = CanMove(*sherlock, m, &last);

    while (can != 1)
    {

        printf("not a correct move,try again:           ");
        m = GetTheMove();

        can = CanMove(*sherlock, m, &last);
    }
    if (can == 1)
    {
        strcpy(sherlock->place->who, "NU");
        sherlock->place = last;
        strcpy(last->who, "SH");
        sherlock->played = 1;
        srand(time(NULL));
        int random = rand() % 8;
        person *pick;
        int i = 0;
        for (i = 0; i < random;)
        {
            person *ptr = odd;
            while (ptr != NULL)
            {
                if (ptr->got == 0)
                {
                    i++;
                    if (i == random)
                    {
                        pick = ptr;
                    }
                }
                ptr = ptr->next;
            }
            ptr = even;
            while (ptr != NULL)
            {
                if (ptr->got == 0)
                {
                    i++;
                    if (i == random)
                    {
                        pick = ptr;
                    }
                }
                ptr = ptr->next;
            }
            if (i == 0)
            {
                printf("no more cards");
                break;
            }
        }
        if (i != 0)
        {
            pick->got = 1;
            pick->guilty = 1;
            printf("        %s is innocent", pick->name);
        }
        getchar();
        return;
    }
}

void JW_play()
{
    person *john = FindThePerson("JW");
    johnlight_off(john->place, johndirection);
    printf("JW : move 1 to 3 cells : ");
    cell *last;
    char *m = GetTheMove();
    int can = CanMove(*john, m, &last);
    while (can != 1)
    {
        printf("%d", can);
        printf(" not a correct move,try again:           ");
        m = GetTheMove();
        can = CanMove(*john, m, &last);
    }
    if (can == 1)
    {
        strcpy(john->place->who, "NU");
        john->place = last;
        strcpy(last->who, "JW");
        john->played = 1;
    }

    printf("choose the direction you want to lighten(i,u,k,m,n,h):           ");
    char move;
    scanf(" %c", &move);

    if (move == 'u')
    {
        johnlight(john->place, 0);

        johndirection = 0;
    }
    else if (move == 'i')
    {
        johnlight(john->place, 1);
        johndirection = 1;
    }
    else if (move == 'k')
    {
        johnlight(john->place, 2);
        johndirection = 2;
    }
    else if (move == 'm')
    {
        johnlight(john->place, 3);
        johndirection = 3;
    }
    else if (move == 'n')
    {
        johnlight(john->place, 4);
        johndirection = 4;
    }
    else if (move == 'h')
    {
        johnlight(john->place, 5);
        johndirection = 5;
    }

    return;
}

void JS_play()
{
    person *john = FindThePerson("JS");
    printf("JS : choose: 1)turn off 1 lamp and turn on another          2)move 1 to 3 steps             ");
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {
        printf("choose the lamp you want to turn off(a number) :                 ");
        int off;
        scanf("%d", &off);
        printf("choose the lamp you want to turn on(a number) :                 ");
        int on;
        scanf("%d", &on);
        while (off == on)
        {
            printf("lamps must be different");
            printf("choose the lamp you want to turn off(a number) :           ");
            scanf("%d", &off);
            printf("choose the lamp you want to turn on(a number) :           ");
            scanf("%d", &on);
        }

        if (off == 1)
        {
            strcpy(lamp[0]->what, "lamp1-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[0]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[0]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 2)
        {
            strcpy(lamp[1]->what, "lamp2-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[1]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[1]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 3)
        {
            strcpy(lamp[2]->what, "lamp3-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[2]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[2]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 4)
        {
            strcpy(lamp[3]->what, "lamp4-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[3]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[3]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 5)
        {
            strcpy(lamp[4]->what, "lamp5-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[4]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[4]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 6)
        {
            strcpy(lamp[5]->what, "lamp6-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[5]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[5]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 7)
        {
            strcpy(lamp[6]->what, "lamp7-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[6]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[6]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 8)
        {
            strcpy(lamp[7]->what, "lamp8-on!");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[7]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[7]->neighbor[i]->watch, "off");
            }
        }
        off = on;
        if (off == 1)
        {
            strcpy(lamp[0]->what, "lamp1-on!");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[0]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[0]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 2)
        {
            strcpy(lamp[1]->what, "lamp2-on!");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[1]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[1]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 3)
        {
            strcpy(lamp[2]->what, "lamp3-on!");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[2]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[2]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 4)
        {
            strcpy(lamp[3]->what, "lamp4-on!");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[3]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[3]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 5)
        {
            strcpy(lamp[4]->what, "lamp5-on!");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[4]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[4]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 6)
        {
            strcpy(lamp[5]->what, "lamp6-on!");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[5]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[5]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 7)
        {
            strcpy(lamp[6]->what, "lamp7-on!");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[6]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[6]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 8)
        {
            strcpy(lamp[7]->what, "lamp8-on!");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[7]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[7]->neighbor[i]->watch, "on!");
            }
        }

        printf("JS : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*john, m, &last);
        while (can != 1)
        {
            printf("not a correct move,try again:       ");
            m = GetTheMove();
            can = CanMove(*john, m, &last);
        }
        if (can == 1)
        {
            strcpy(john->place->who, "NU");
            john->place = last;
            strcpy(last->who, "JS");
            john->played = 1;
        }
    }
    else if (choice == 2)
    {
        printf("JS : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*john, m, &last);
        while (can != 1)
        {
            printf("not a correct move,try again:        ");
            m = GetTheMove();
            can = CanMove(*john, m, &last);
        }
        if (can == 1)
        {
            strcpy(john->place->who, "NU");
            john->place = last;
            strcpy(last->who, "JS");
            john->played = 1;
        }
        printf("choose the lamp you want to turn off(a number) :          ");
        int off;
        scanf("%d", &off);
        printf("choose the lamp you want to turn on(a number) :           ");
        int on;
        scanf("%d", &on);
        while (off == on)
        {
            printf("lamps must be different");
            printf("choose the lamp you want to turn off(a number) :          ");
            scanf("%d", &off);
            printf("choose the lamp you want to turn on(a number) :          ");
            scanf("%d", &on);
        }

        if (off == 1)
        {

            strcpy(lamp[0]->what, "lamp1-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[0]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[0]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 2)
        {
            strcpy(lamp[1]->what, "lamp2-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[1]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[1]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 3)
        {
            strcpy(lamp[2]->what, "lamp3-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[2]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[2]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 4)
        {
            strcpy(lamp[3]->what, "lamp4-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[3]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[3]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 5)
        {
            strcpy(lamp[4]->what, "lamp5-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[4]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[4]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 6)
        {
            strcpy(lamp[5]->what, "lamp6-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[5]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[6]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 7)
        {
            strcpy(lamp[6]->what, "lamp7-off");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[6]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[6]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 8)
        {
            strcpy(lamp[7]->what, "lamp8-on!");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[7]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[7]->neighbor[i]->watch, "off");
            }
        }
        off = on;
        if (off == 1)
        {
            strcpy(lamp[0]->what, "lamp1-on!");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[0]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[0]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 2)
        {
            strcpy(lamp[1]->what, "lamp2-on!");
            for (int i = 0; i < 6; i++)
            {
                if (lamp[1]->neighbor[i] == NULL)
                {
                    continue;
                }
                strcpy(lamp[1]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 3)
        {
            strcpy(lamp[2]->what, "lamp3-on!");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[2]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 4)
        {
            strcpy(lamp[3]->what, "lamp4-on!");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[3]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 5)
        {
            strcpy(lamp[4]->what, "lamp5-on!");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[4]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 6)
        {
            strcpy(lamp[5]->what, "lamp6-on!");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[5]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 7)
        {
            strcpy(lamp[6]->what, "lamp7-on!");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[6]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 8)
        {
            strcpy(lamp[7]->what, "lamp8-on!");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[7]->neighbor[i]->watch, "on!");
            }
        }
    }
    johnlight((FindThePerson("JW")->place), johndirection);
    return;
}

void SG_play()
{
    person *goodley = FindThePerson("SG");
    printf("SG : choose: 1)call person(s) 3 cells closer           2)move 1 to 3 steps      ");
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {

        for (int i = 0; i < 3; i++)
        {
            printf("insert the person's initials:            ");
            char close[3];
            char *ptr = close;
            scanf(" %s", close);
            person *called = FindThePerson(ptr);
            char move;
            char themove[2] = "a";
            if (called->place->y == goodley->place->y && called->place->x > goodley->place->x)
            {
                printf("allowed moves(choose one): u :       ");
                scanf(" %c", &move);
                while (move != 'u')
                {
                    getchar();
                    printf("invalid choice, try again:         ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y == goodley->place->y && called->place->x < goodley->place->x)
            {
                printf("allowed moves(choose one): m :       ");
                scanf(" %c", &move);
                while (move != 'm')
                {
                    getchar();
                    printf("\n invalid choice, try again:   ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y > goodley->place->y && called->place->x == goodley->place->x)
            {
                printf("allowed moves(choose one): h , n :       ");
                scanf(" %c", &move);
                while (move != 'h' && move != 'n')
                {
                    getchar();
                    printf("\n invalid choice, try again:   ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y < goodley->place->y && called->place->x == goodley->place->x)
            {
                printf("allowed moves(choose one): k , i :       ");
                scanf(" %c", &move);
                while (move != 'i' && move != 'k')
                {
                    getchar();
                    printf("\n invalid choice, try again:   ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y > goodley->place->y && called->place->x > goodley->place->x)
            {
                printf("allowed moves(choose one): u , h , n :       ");
                scanf(" %c", &move);
                while (move != 'u' && move != 'h' && move != 'n')
                {
                    getchar();
                    printf("invalid choice, try again:            ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y > goodley->place->y && called->place->x < goodley->place->x)
            {
                printf("allowed moves(choose one): h , n , m :       ");
                scanf(" %c", &move);
                while (move != 'h' && move != 'n' && move != 'm')
                {
                    getchar();
                    printf("invalid choice, try again:         ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y < goodley->place->y && called->place->x > goodley->place->x)
            {
                printf("allowed moves(choose one): u , i , k :       ");
                scanf(" %c", &move);
                while (move != 'u' && move != 'i' && move != 'k')
                {
                    getchar();
                    printf("invalid choice, try again:           ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y < goodley->place->y && called->place->x < goodley->place->x)
            {
                printf("allowed moves(choose one): i , k ,m :       ");
                scanf(" %c", &move);
                while (move != 'i' && move != 'k' && move != 'm')
                {
                    getchar();
                    printf(" invalid choice, try again:            ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            cell *last;
            int can = CanMove(*called, &(themove[0]), &last);
            while (can != 1)
            {
                printf("not a correct move,try again:            ");
                char *m = GetTheMove();
                can = CanMove(*called, m, &last);
            }
            if (can == 1)
            {
                if (!strcmp(called->name, "JW"))
                {
                    johnlight_off(called->place, johndirection);
                }
                strcpy(called->place->who, "NU");
                called->place = last;
                strcpy(last->who, called->name);
            }
            if (!strcmp(called->name, "JW"))
            {
                johnlight(called->place, johndirection);
            }
        }

        printf("SG : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*goodley, m, &last);
        while (can != 1)
        {
            printf("not a correct move,try again:           ");
            m = GetTheMove();
            can = CanMove(*goodley, m, &last);
        }
        if (can == 1)
        {
            strcpy(goodley->place->who, "NU");
            goodley->place = last;
            strcpy(last->who, "SG");
            goodley->played = 1;
        }
    }
    else if (choice == 2)
    {
        printf("SG : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*goodley, m, &last);
        while (can != 1)
        {
            printf("not a correct move,try again:            ");
            m = GetTheMove();
            can = CanMove(*goodley, m, &last);
        }
        if (can == 1)
        {
            strcpy(goodley->place->who, "NU");
            goodley->place = last;
            strcpy(last->who, "SG");
            goodley->played = 1;
        }
        system("cls");
        display_map();

        for (int i = 0; i < 3; i++)
        {
            printf("insert the person's initials:          ");
            char close[3];
            char *ptr = close;
            scanf(" %s", close);
            person *called = FindThePerson(ptr);
            printf("%s", called->name);
            char move;
            char themove[2] = " ";
            if (called->place->y == goodley->place->y && called->place->x > goodley->place->x)
            {
                printf("allowed moves(choose one): u :       ");
                scanf(" %c", &move);
                while (move != 'u')
                {
                    printf("invalid choice, try again:            ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y == goodley->place->y && called->place->x < goodley->place->x)
            {
                printf("allowed moves(choose one): m :       ");
                scanf(" %c", &move);
                while (move != 'm')
                {
                    printf(" invalid choice, try again:           ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y > goodley->place->y && called->place->x == goodley->place->x)
            {
                printf("allowed moves(choose one): h , n :       ");
                scanf(" %c", &move);
                while (move != 'h' && move != 'n')
                {
                    printf("invalid choice, try again:            ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y < goodley->place->y && called->place->x == goodley->place->x)
            {
                printf("allowed moves(choose one): k , i :       ");
                scanf(" %c", &move);
                while (move != 'i' && move != 'k')
                {
                    printf(" invalid choice, try again:          ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y > goodley->place->y && called->place->x > goodley->place->x)
            {
                printf("allowed moves(choose one): u , h , n :       ");
                scanf(" %c", &move);
                while (move != 'u' && move != 'h' && move != 'n')
                {
                    printf("invalid choice, try again:            ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y > goodley->place->y && called->place->x < goodley->place->x)
            {
                printf("allowed moves(choose one): h , n , m :       ");
                scanf(" %c", &move);
                while (move != 'h' && move != 'n' && move != 'm')
                {
                    printf("invalid choice, try again:          ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y < goodley->place->y && called->place->x > goodley->place->x)
            {
                printf("allowed moves(choose one): u , i , k :       ");
                scanf(" %c", &move);
                while (move != 'u' && move != 'i' && move != 'k')
                {
                    printf("invalid choice, try again:            ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            else if (called->place->y < goodley->place->y && called->place->x < goodley->place->x)
            {
                printf("allowed moves(choose one): i , k ,m :       ");
                scanf(" %c", &move);
                while (move != 'i' && move != 'k' && move != 'm')
                {
                    printf("invalid choice, try again:         ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            cell *last;
            int can = CanMove(*called, &(themove[0]), &last);
            while (can != 1)
            {
                printf("not a correct move,try again:           ");
                char *m = GetTheMove();
                while (strlen(m) != 1)
                {
                    printf("not a correct move,try again:         ");
                    m = GetTheMove();
                }
                can = CanMove(*called, m, &last);
            }
            if (can == 1)
            {
                strcpy(called->place->who, "NU");
                called->place = last;
                strcpy(last->who, called->name);
            }
        }
    }
    return;
}

void IL_play()
{
    person *lesterade = FindThePerson("IL");
    printf("IL : choose: 1)close one exit and open another          2)move 1 to 3 steps         ");
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {
        printf("choose the exit you want to close(1-4) :      ");
        int off;
        scanf("%d", &off);
        printf("choose the exit you want to open(1-4) :       ");
        int on;
        scanf("%d", &on);
        while (off == on)
        {
            printf("lamps must be different");
            printf("choose the exit you want to close(1-4) :          ");
            scanf("%d", &off);
            printf("choose the exit you want to close(1-4) :          ");
            scanf("%d", &on);
        }
        if (off == 1)
        {
            strcpy(out[0]->what, "out1-clsd");
        }
        else if (off == 2)
        {
            strcpy(out[1]->what, "out2-clsd");
        }
        else if (off == 3)
        {
            strcpy(out[2]->what, "out3-clsd");
        }
        else if (off == 4)
        {
            strcpy(out[3]->what, "out4-clsd");
        }
        off = on;
        if (off == 1)
        {
            strcpy(out[0]->what, "out1-open");
        }
        else if (off == 2)
        {
            strcpy(out[1]->what, "out2-open");
        }
        else if (off == 3)
        {
            strcpy(out[2]->what, "out3-open");
        }
        else if (off == 4)
        {
            strcpy(out[3]->what, "out4-open");
        }
        printf("IL : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*lesterade, m, &last);
        while (can != 1)
        {
            printf("not a correct move,try again:        ");
            m = GetTheMove();
            can = CanMove(*lesterade, m, &last);
            printf("%d", can);
        }
        if (can == 1)
        {
            strcpy(lesterade->place->who, "NU");
            lesterade->place = last;
            strcpy(last->who, "IL");
            lesterade->played = 1;
        }
    }
    else if (choice == 2)
    {
        printf("IL : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*lesterade, m, &last);
        while (can != 1)
        {
            printf("not a correct move,try again:           ");
            m = GetTheMove();
            can = CanMove(*lesterade, m, &last);
            printf("%d", can);
        }
        if (can == 1)
        {
            strcpy(lesterade->place->who, "NU");
            lesterade->place = last;
            strcpy(last->who, "IL");
            lesterade->played = 1;
        }
        printf("choose the exit you want to close(1-4) :          ");
        int off;
        scanf("%d", &off);
        printf("choose the exit you want to open(1-4) :            ");
        int on;
        scanf("%d", &on);
        while (off == on)
        {
            printf("lamps must be different");
            printf("choose the exit you want to close(1-4) :       ");
            scanf("%d", &off);
            printf("choose the exit you want to close(1-4) :      ");
            scanf("%d", &on);
        }
        if (off == 1)
        {
            strcpy(out[0]->what, "out1-clsd");
        }
        else if (off == 2)
        {
            strcpy(out[1]->what, "out2-clsd");
        }
        else if (off == 3)
        {
            strcpy(out[2]->what, "out3-clsd");
        }
        else if (off == 4)
        {
            strcpy(out[3]->what, "out4-clsd");
        }
        off = on;
        if (off == 1)
        {
            strcpy(out[0]->what, "out1-open");
        }
        else if (off == 2)
        {
            strcpy(out[1]->what, "out2-open");
        }
        else if (off == 3)
        {
            strcpy(out[2]->what, "out3-open");
        }
        else if (off == 4)
        {
            strcpy(out[3]->what, "out4-open");
        }
    }
    return;
}

void MS_play()
{
    person *stealthy = FindThePerson("MS");
    printf("MS : move 1 to 4 cells (you can move through everything): ");
    cell *last;
    char *m = GetTheMove();
    int can = CanMove(*stealthy, m, &last);
    while (can != 1)
    {
        printf("not a correct move,try again:           ");
        m = GetTheMove();
        can = CanMove(*stealthy, m, &last);
        printf("%d", can);
    }
    if (can == 1)
    {
        strcpy(stealthy->place->who, "NU");
        stealthy->place = last;
        strcpy(last->who, "MS");
        stealthy->played = 1;
    }
    return;
}

void WG_play()
{
    person *wili = FindThePerson("WG");
    printf("WG : choose: 1)change places with someone    *or*     2)move 1 to 3 steps       ");
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {
        printf("please insert the person's initials you want to change places with:      ");
        char select[3];
        char *ptr = select;
        scanf(" %s", ptr);
        person *changewith = FindThePerson(ptr);
        if (!strcmp(ptr, "JW"))
        {
            johnlight_off(changewith->place, johndirection);
        }
        cell *temp = wili->place;
        wili->place = changewith->place;
        wili->played = 1;
        changewith->place = temp;
        strcpy(changewith->place->who, ptr);
        strcpy(wili->place->who, "WG");
        if (!strcmp(ptr, "JW"))
        {
            johnlight(changewith->place, johndirection);
        }
    }
    else if (choice == 2)
    {
        printf("WG : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*wili, m, &last);
        while (can != 1)
        {
            printf(" not a correct move,try again:          ");
            m = GetTheMove();
            can = CanMove(*wili, m, &last);
        }
        if (can == 1)
        {
            strcpy(wili->place->who, "NU");
            wili->place = last;
            strcpy(last->who, "WG");
            wili->played = 1;
        }
    }
    return;
}

void JB_play()
{
    person *jeremy = FindThePerson("JB");
    printf("JB : choose: 1)close 1 tunnel and open another           2)move 1 to 3 steps        ");
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {
        printf("choose the tunnel you want to close(a number) :           ");
        int off;
        scanf("%d", &off);
        printf("choose the tunnel you want to open(a number) :            ");
        int on;
        scanf("%d", &on);
        while (off == on)
        {
            printf("tunnels must be different");
            printf("choose the tunnel you want to turn off(a number) :        ");
            scanf("%d", &off);
            printf("choose the tunnel you want to turn on(a number) :         ");
            scanf("%d", &on);
        }

        if (off == 1)
        {
            strcpy(tunnel[0]->what, "tnl1-clsd");
        }
        else if (off == 2)
        {
            strcpy(tunnel[1]->what, "tnl2-clsd");
        }
        else if (off == 3)
        {
            strcpy(tunnel[2]->what, "tnl3-clsd");
        }
        else if (off == 4)
        {
            strcpy(tunnel[3]->what, "tnl4-clsd");
        }
        else if (off == 5)
        {
            strcpy(tunnel[4]->what, "tnl5-clsd");
        }
        else if (off == 6)
        {
            strcpy(tunnel[5]->what, "tnl6-clsd");
        }
        else if (off == 7)
        {
            strcpy(tunnel[6]->what, "tnl7-clsd");
        }
        else if (off == 8)
        {
            strcpy(tunnel[7]->what, "tnl8-clsd");
        }
        off = on;
        if (off == 1)
        {
            strcpy(tunnel[0]->what, "tnl1-open");
        }
        else if (off == 2)
        {
            strcpy(tunnel[1]->what, "tnl2-open");
        }
        else if (off == 3)
        {
            strcpy(tunnel[2]->what, "tnl3-open");
        }
        else if (off == 4)
        {
            strcpy(tunnel[3]->what, "tnl4-open");
        }
        else if (off == 5)
        {
            strcpy(tunnel[4]->what, "tnl5-open");
        }
        else if (off == 6)
        {
            strcpy(tunnel[5]->what, "tnl6-open");
        }
        else if (off == 7)
        {
            strcpy(tunnel[6]->what, "tnl7-open");
        }
        else if (off == 8)
        {
            strcpy(tunnel[7]->what, "tnl8-open");
        }

        printf("JB : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*jeremy, m, &last);
        while (can != 1)
        {
            printf(" not a correct move,try again:          ");
            m = GetTheMove();
            can = CanMove(*jeremy, m, &last);
        }
        if (can == 1)
        {
            strcpy(jeremy->place->who, "NU");
            jeremy->place = last;
            strcpy(last->who, "JB");
            jeremy->played = 1;
        }
    }
    else if (choice == 2)
    {
        printf("JB : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*jeremy, m, &last);
        while (can != 1)
        {
            printf("not a correct move,try again:           ");
            m = GetTheMove();
            can = CanMove(*jeremy, m, &last);
        }
        if (can == 1)
        {
            strcpy(jeremy->place->who, "NU");
            jeremy->place = last;
            strcpy(last->who, "JB");
            jeremy->played = 1;
        }
        printf("choose the tunnel you want to close(a number) :           ");
        int off;
        scanf("%d", &off);
        printf("choose the tunnel you want to open(a number) :             ");
        int on;
        scanf("%d", &on);
        while (off == on)
        {
            printf("tunnels must be different");
            printf("choose the tunnel you want to turn off(a number) :             ");
            scanf("%d", &off);
            printf("choose the tunnel you want to turn on(a number) :             ");
            scanf("%d", &on);
        }

        if (off == 1)
        {
            strcpy(tunnel[0]->what, "tnl1-clsd");
        }
        else if (off == 2)
        {
            strcpy(tunnel[1]->what, "tnl2-clsd");
        }
        else if (off == 3)
        {
            strcpy(tunnel[2]->what, "tnl3-clsd");
        }
        else if (off == 4)
        {
            strcpy(tunnel[3]->what, "tnl4-clsd");
        }
        else if (off == 5)
        {
            strcpy(tunnel[4]->what, "tnl5-clsd");
        }
        else if (off == 6)
        {
            strcpy(tunnel[5]->what, "tnl6-clsd");
        }
        else if (off == 7)
        {
            strcpy(tunnel[6]->what, "tnl7-clsd");
        }
        else if (off == 8)
        {
            strcpy(tunnel[7]->what, "tnl8-clsd");
        }
        off = on;
        if (off == 1)
        {
            strcpy(tunnel[0]->what, "tnl1-open");
        }
        else if (off == 2)
        {
            strcpy(tunnel[1]->what, "tnl2-open");
        }
        else if (off == 3)
        {
            strcpy(tunnel[2]->what, "tnl3-open");
        }
        else if (off == 4)
        {
            strcpy(tunnel[3]->what, "tnl4-open");
        }
        else if (off == 5)
        {
            strcpy(tunnel[4]->what, "tnl5-open");
        }
        else if (off == 6)
        {
            strcpy(tunnel[5]->what, "tnl6-open");
        }
        else if (off == 7)
        {
            strcpy(tunnel[6]->what, "tnl7-open");
        }
        else if (off == 8)
        {
            strcpy(tunnel[7]->what, "tnl8-open");
        }
    }
    return;
}

void MainMenu()
{
    system("cls");
    SetColor(3);
    printf("\n\n          >>>>Mr.Jack<<<<\n");
    SetColor(4);
    printf("          1) Resume \n");
    SetColor(2);
    printf("          2) New Game\n");
    SetColor(13);
    printf("          3) Scoreboard\n");
    SetColor(1);
    printf("          4) Clear Scorebord\n");
    SetColor(14);
    printf("          5) Tutorial\n          ");
    SetColor(15);
    int temp;
    scanf("%d", &temp);
    getchar();
    if (temp == 1)
    {
        resumethegame();
        while (turnglobal < 5)
        {
            newturn();
            turnglobal++;
            checkend();
            savethegame();
        }
        roundglobal++;
        if (roundglobal == 9)
        {
            loose();
        }
        checkvisibility();
        NewRound();
    }
    else if (temp == 2)
    {
        create_cards();
        shuffle();
        GetDeafultGame();
        system("cls");
        SetColor(13);
        for (int i = 0; i < 19; i++)
        {
            decname[i] = '-';
            jackname[i] = '-';
        }
        decname[19] = '\0';
        jackname[19] = '\0';
        printf("\n\n    enter your name Jack's player:    ");
        char c1;
        char *ptr = jackname;
        c1 = getchar();
        while (isprint((int)(c1)) || jackname[0] == '-')
        {
            *ptr = c1;
            ptr = ptr + 1;
            c1 = getchar();
        }
        system("cls");
        SetColor(9);
        printf("\n\n    enter your name Detective's player:    ");
        ptr = decname;
        c1 = getchar();
        while (isprint((int)(c1)) || decname[0] == '-')
        {
            *ptr = c1;
            ptr = ptr + 1;
            c1 = getchar();
        }
        system("cls");
        printf("\n\n    It is Jack's turn. Jack please press 1 to see your identity     ");
        srand(time(NULL));
        int random = rand() % 8;
        person *pick = odd;
        if (random <= 3)
        {
            for (int i = 0; i < random; i++)
            {
                pick = pick->next;
            }
        }
        else
        {
            pick = even;
            for (int i = 4; i < random; i++)
            {
                pick = pick->next;
            }
        }
        int t;
        scanf("%d", &t);
        getchar();
        printf("\n\n    Jack, You are disguised as %s\n", pick->name);
        pick->got = 1;
        jackglobal = pick;
        getchar();
        system("cls");
        NewRound();
    }
    else if (temp == 5)
    {
        system("cls");
        printf("\n\n\n    how to move players:\n");
        printf("               _______\n");
        printf("              /       \\\n");
        printf("      _______/    u    \\_______\n");
        printf("     /       \\         /       \\\n");
        printf("    /    h    \\_______/    i    \\\n");
        printf("    \\         /       \\         /\n");
        printf("     \\_______/    j    \\_______/\n");
        printf("     /       \\your cell/       \\\n");
        printf("    /    n    \\_______/    k    \\\n");
        printf("    \\         /       \\         /\n");
        printf("     \\_______/    m    \\_______/\n");
        printf("             \\         /       \n");
        printf("              \\_______/\n\n");
        printf("    for example to go from cell A to B you should type:  uh       \n");
        printf("               _______\n");
        printf("              /       \\\n");
        printf("      _______/         \\_______\n");
        printf("     /       \\         /       \\\n");
        printf("    /    B    \\_______/         \\\n");
        printf("    \\         /       \\         /\n");
        printf("     \\_______/h        \\_______/\n");
        printf("     /       \\         /       \\\n");
        printf("    /         \\___u___/         \\\n");
        printf("    \\         /       \\         /\n");
        printf("     \\_______/    A    \\_______/\n");
        printf("             \\         /       \n");
        printf("              \\_______/\n\n");
        printf("    for using the tunnels you type:  j + the number of your destination tunnel     \n");
        printf("    for example to go from cell A to B you should type:  ij3n       \n");
        printf("               _______\n");
        printf("              /       \\\n");
        printf("      _______/tnl3-open\\_______\n");
        printf("     /       \\   3     /       \\\n");
        printf("    /    B   n\\_______/         \\\n");
        printf("    \\         /       \\         /\n");
        printf("     \\_______/         \\_______/\n");
        printf("     /       \\         /       \\\n");
        printf("    /         \\_______/tnl6-open\\\n");
        printf("    \\         /       \\    j    /\n");
        printf("     \\_______/    A   i\\_______/\n");
        printf("             \\         /       \n");
        printf("              \\_______/\n\n");
        printf("    i hope you have a good time with your friends anf families playing my game :*\n\n");
        SetColor(12);
        printf("    coded by Helia Akbari");
        printf("\n    contact me: https://github.com/heliaakbari\n\n\n");
        SetColor(15);
        system("pause");
        system("cls");
        MainMenu();
    }
    else if (temp == 3)
    {
        system("cls");
        printf("\n\n          scoreboard      \n");
        FILE *fp = fopen("scoreboard.bin", "rb");
        char name[20];
        int num;
        int score;
        fread(&num, sizeof(int), 1, fp);
        if (num == 0)
        {
            printf("        no records yet\n");
        }
        while (1)
        {
            fread(name, sizeof(char), 20, fp);
            fread(&score, sizeof(int), 1, fp);
            if (feof(fp))
            {
                break;
            }
            printf("     %s", name);
            printf("%d \n", score);
        }
        fclose(fp);
        printf("\n\n");
        system("pause");
        MainMenu();
    }
    else if (temp == 4)
    {
        FILE *fp = fopen("scoreboard.bin", "wb");
        int n = 0;
        fwrite(&n, sizeof(int), 1, fp);
        fclose(fp);
        MainMenu();
    }
    return;
}

void win()
{
    system("cls");
    printf("\n\n           The Detective Wins!\n\n\n\n");
    FILE *fp = fopen("scoreboard.bin", "rb+");
    if (fp == NULL)
    {
        printf("error1");
    }
    int num = 0;
    int wins = 0;
    char name[20];
    fread(&num, sizeof(int), 1, fp);
    int mark = 0;
    while (1)
    {
        fread(name, sizeof(char), 20, fp);
        fread(&wins, sizeof(int), 1, fp);
        if (feof(fp))
        {
            break;
        }
        if (!strncmp(decname, name, 18))
        {
            wins++;
            fseek(fp, -4, SEEK_CUR);
            fwrite(&wins, sizeof(int), 1, fp);
            mark = 1;
            break;
        }
    }
    if (mark == 0)
    {
        wins = 1;
        fclose(fp);
        fp = fopen("scoreboard.bin", "ab");
        fwrite(decname, sizeof(char), 20, fp);
        fwrite(&wins, sizeof(int), 1, fp);
        fclose(fp);
        fp = fopen("scoreboard.bin", "rb+");
        if (fp == NULL)
        {
            printf("error2");
        }
        num++;
        fwrite(&num, sizeof(int), 1, fp);
    }
    fclose(fp);
    system("pause");
    exit(0);
}

void loose()
{
    system("cls");
    printf("\n\n            Jack Wins!");
    printf("jack's secret identity was %s\n\n\n\n!", jackglobal->name);
    FILE *fp = fopen("scoreboard.bin", "rb+");
    if (fp == NULL)
    {
        printf("error1");
    }
    int num = 0;
    int wins = 0;
    char name[20];
    fread(&num, sizeof(int), 1, fp);
    int mark = 0;
    while (1)
    {
        fread(name, sizeof(char), 20, fp);
        fread(&wins, sizeof(int), 1, fp);
        if (feof(fp))
        {
            break;
        }
        if (!strncmp(jackname, name, 18))
        {
            wins++;
            fseek(fp, -4, SEEK_CUR);
            fwrite(&wins, sizeof(int), 1, fp);
            mark = 1;
            break;
        }
    }
    if (mark == 0)
    {
        wins = 1;
        fclose(fp);
        fp = fopen("scoreboard.bin", "ab");
        fwrite(jackname, sizeof(char), 20, fp);
        fwrite(&wins, sizeof(int), 1, fp);
        fclose(fp);
        fp = fopen("scoreboard.bin", "rb+");
        if (fp == NULL)
        {
            printf("error2");
        }
        num++;
        fwrite(&num, sizeof(int), 1, fp);
    }
    fclose(fp);
    system("pause");
    exit(0);
}