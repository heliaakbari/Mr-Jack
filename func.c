#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "func.h"
#define wallc 8
#define whatc 2
#define watchc 1
#define whoc 3

int turnglobal = 1;
int roundglobal = 1;
person *jackglobal;

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

void display_map(cell map[9][13], person *odd, person *even)
{
    SetColor(wallc);

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

    if (turnglobal % 8 == 1 || turnglobal % 8 == 4 || turnglobal % 8 == 6 || turnglobal % 8 == 7)
    {
        printf("  Detective's turn(round%d,turn%d)", roundglobal, turnglobal);
    }
    else
    {
        printf("  Jacks's turn(round%d,turn%d)", roundglobal, turnglobal);
    }
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
    if (roundglobal == 1 && turnglobal == 1)
    {
        printf("    Jack is visible");
    }
    else
    {
        if (jackglobal->watch == 0)
        {
            printf("    Jack is invisible");
        }
        else
        {
            printf("    Jack is visible");
        }
    }

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
        printf("       \\___");
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
        printf("____/");
    }
    printf("             innocents: ");
    person *ptr = odd;
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

void GetDeafultGame(cell map[9][13])
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
                map[i][j].neighbor[k] = &map[t1][t2];
            }
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
    odd->got = 0;
    odd->watch = 1;
    odd->guilty = 0;
    odd->place = &map[6][6];
    strcpy(odd->ability, "identify an innocent");
    odd->next = (person *)malloc(sizeof(person));
    odd = odd->next;
    strncpy(odd->name, "JW", 3);
    odd->played = 0;
    odd->got = 0;
    odd->guilty = 0;
    odd->watch = 0;
    odd->place = &map[5][0];
    strcpy(odd->ability, "iluminate 1 direction");
    odd->next = (person *)malloc(sizeof(person));
    odd = odd->next;
    strncpy(odd->name, "JS", 3);
    odd->played = 0;
    odd->got = 0;
    odd->guilty = 0;
    odd->watch = 1;
    odd->place = &map[3][6];
    strcpy(odd->ability, "turn off 1 lamp and turn on another");
    odd->next = (person *)malloc(sizeof(person));
    odd = odd->next;
    strncpy(odd->name, "IL", 3);
    odd->played = 0;
    odd->got = 0;
    odd->guilty = 0;
    odd->watch = 1;
    odd->place = &map[5][4];
    strcpy(odd->ability, "open 1 exit and close another");
    odd->next = NULL;
    *ptr_odd = list1;

    person *list2 = (person *)malloc(sizeof(person));
    odd = list2;
    strncpy(odd->name, "MS", 3);
    odd->played = 0;
    odd->got = 0;
    odd->guilty = 0;
    odd->watch = 0;
    odd->place = &map[8][8];
    strcpy(odd->ability, "move through apartments but can't stop in them");
    odd->next = (person *)malloc(sizeof(person));
    odd = odd->next;
    strncpy(odd->name, "SG", 3);
    odd->played = 0;
    odd->watch = 0;
    odd->guilty = 0;
    odd->got = 0;
    odd->place = &map[4][12];
    strcpy(odd->ability, "call chracter(s) 3 cells closer to himself");
    odd->next = (person *)malloc(sizeof(person));
    odd = odd->next;
    strncpy(odd->name, "WG", 3);
    odd->played = 0;
    odd->guilty = 0;
    odd->got = 0;
    odd->watch = 0;
    odd->place = &map[0][4];
    strcpy(odd->ability, "change place with another chracter");
    odd->next = (person *)malloc(sizeof(person));
    odd = odd->next;
    strncpy(odd->name, "JB", 3);
    odd->played = 0;
    odd->guilty = 0;
    odd->got = 0;
    odd->watch = 1;
    odd->place = &map[4][8];
    strcpy(odd->ability, "open 1 tunnel and close another");
    odd->next = NULL;
    *ptr_even = list2;
    return;
}

void shuffle(person **ptr_odd, person **ptr_even)
{
    // srand(time(NULL));
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

person *FindThePerson(person *odd, person *even, char names[3])
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
    char c = getchar();
    char *des = (char *)malloc(10 * sizeof(char));
    for (int i = 0; i < 10; i++)
    {
        des[i] = '\0';
    }
    while (c != '\0' || c != '\n')
    {
        if (c == 'u' || c == 'i' || c == 'k' || c == 'm' || c == 'n' || c == 'h' || c == 'j' || (c >= '1' && c <= '8' && des[strlen(des) - 1] == 'j'))
        {
            strcat(des, &c);
        }
        else if (c == ' ')
        {
            ;
        }
        else
        {
            printf("not a valid command, please try again using(u,i,k,m,n,h,j,1-8)");
            char *ptr = GetTheMove();
            strcpy(des, ptr);
        }
        c = getchar();
    }
    des[strlen(des)] = '\n';
    return des;
}

int CanMove(cell map[9][13], person mover, char *move, cell *tunnel[8], cell **last)
{
    char *ptr = move;
    int count = 0;
    cell *where = mover.place;
    int total = 3;
    if (!strcmp(mover.name, "MS"))
    {
        total = 4;
    }
    while (*ptr != '\n')
    {
        if (*ptr == 'u')
        {
            where = where->neighbor[0];
            if (where->x == -1)
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
                    return -1;
                }
            }
        }
        else if (*ptr == 'i')
        {
            where = where->neighbor[1];
            if (where->x == -1)
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
                    return -1;
                }
            }
        }
        else if (*ptr == 'k')
        {
            where = where->neighbor[2];
            if (where->x == -1)
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
                    return -1;
                }
            }
        }
        else if (*ptr == 'm')
        {
            where = where->neighbor[3];
            if (where->x == -1)
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
                    return -1;
                }
            }
        }
        else if (*ptr == 'n')
        {
            where = where->neighbor[4];
            if (where->x == -1)
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
                    return -1;
                }
            }
        }
        else if (*ptr == 'h')
        {
            where = where->neighbor[5];
            if (where->x == -1)
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
                    return -1;
                }
            }
        }
        else if (*ptr == 'j')
        {
            if (strncmp(&(where->what[5]), "open", 3))
            {
                return -1;
            }
            count--;
        }
        else if (*ptr >= '1' && *ptr <= '8')
        {
            int num = (int)(*ptr) - 49;
            where = tunnel[num];
            if (strncmp(&(where->what[5]), "open", 3))
            {
                return -1;
            }
        }
        count++;

        if (count > total)
        {
            return -1;
        }
    }
    if (strncmp(where->what, "side-walk", 4) && strncmp(where->what, "out1", 3))
    {
        return -1;
    }
    if (strcmp(where->who, "NU"))
    {
        return -1;
    }

    *last = where;
    return 1;
}

cell *checkNear(cell *where)
{
    for (int i = 0; i < 6; i++)
    {
        if (strcmpi(where->neighbor[i]->who, "NU"))
        {
            return where->neighbor[i];
        }
    }
    return NULL;
}

void SH_play(cell map[9][13], cell *tunnel[8], person *odd, person *even)
{
    cell *last;
    person *sherlock = FindThePerson(odd, even, "SH");
    char *m = GetTheMove();
    int can = CanMove(map, *sherlock, m, tunnel, &last);
    if (can == 1)
    {
        strcpy(sherlock->place->who, "NU");
        sherlock->place = last;
        strcpy(last->who, "SH");
        sherlock->played = 1;
        srand(time(NULL));
        int random = rand() % 8;
        person *pick;
        for (int i = 0; i < random;)
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
        }
        pick->got = 1;
        printf("%s is innocent", pick->name);
    }
}

void NewRound(cell map[9][13], person **ptr_odd, person **ptr_even, cell *tunnel[8])
{
    display_map(map, *ptr_odd, *ptr_even);
    printf("choose one(write the initials-capital letter-and press enter): ");
    if (roundglobal % 2 == 1)
    {
        shuffle(ptr_odd, ptr_even);
        person *ptr = *ptr_odd;
        while (ptr != NULL)
        {
            if (ptr->played == 0)
            {
                printf("%s ", ptr->name);
            }
            ptr = ptr->next;
        }
    }
    else
    { // round%2==0
        person *ptr = *ptr_even;
        while (ptr != NULL)
        {
            if (ptr->played == 0)
            {
                printf("%s ", ptr->name);
            }
            ptr = ptr->next;
        }
    }
    printf(" : ");
    char chosen[3];
    scanf(" %s", chosen);
    if (!strncmp(chosen, "SH", 2))
    {
        SH_play(map, tunnel, *ptr_odd, *ptr_even);
    }
    else if (!strncmp(chosen, "JW", 2))
    {
        JW_play(map, tunnel, *ptr_odd, *ptr_even);
    }
    else if (!strncmp(chosen, "JS", 2))
    {
        JS_play(map, tunnel, *ptr_odd, *ptr_even);
    }
    else if (!strncmp(chosen, "IL", 2))
    {
        IL_play(map, tunnel, *ptr_odd, *ptr_even);
    }
    else if (!strncmp(chosen, "MS", 2))
    {
        MS_play(map, tunnel, *ptr_odd, *ptr_even);
    }
    else if (!strncmp(chosen, "SG", 2))
    {
        SG_play(map, tunnel, *ptr_odd, *ptr_even);
    }
    else if (!strncmp(chosen, "SG", 2))
    {
        WG_play(map, tunnel, *ptr_odd, *ptr_even);
    }
    else if (!strncmp(chosen, "SG", 2))
    {
        JB_play(map, tunnel, *ptr_odd, *ptr_even);
    }
    system("cls");
    display_map(map, *ptr_odd, *ptr_even);
}
void JW_play(cell map[9][13], cell *tunnel[8], person *odd, person *even)
{
    return;
}
void JS_play(cell map[9][13], cell *tunnel[8], person *odd, person *even)
{
    return;
}
void IL_play(cell map[9][13], cell *tunnel[8], person *odd, person *even)
{
    return;
}
void MS_play(cell map[9][13], cell *tunnel[8], person *odd, person *even)
{
    return;
}
void SG_play(cell map[9][13], cell *tunnel[8], person *odd, person *even)
{
    return;
}
void WG_play(cell map[9][13], cell *tunnel[8], person *odd, person *even)
{
    return;
}
void JB_play(cell map[9][13], cell *tunnel[8], person *odd, person *even)
{
    return;
}

void MainMenu(cell map[9][13], person **ptr_odd, person **ptr_even, cell *tunnel[8])
{
    system("cls");
    SetColor(3);
    printf("    >>>>Mr.Jack<<<<\n");
    SetColor(4);
    printf("    1) Resume \n");
    SetColor(2);
    printf("    2) New Game\n");
    SetColor(14);
    printf("    3) Tutorial\n");
    int temp;
    scanf("%d", &temp);
    getchar();
    if (temp == 2)
    {
        create_cards(ptr_odd, ptr_even, map);
        shuffle(ptr_odd, ptr_even);
        GetDeafultGame(map);
        system("cls");
        printf("It is Jack's turn. Jack please press 1 to see your identity\n");
        srand(time(NULL));
        int random = rand() % 8;
        person *pick = *ptr_odd;
        if (random <= 3)
        {
            for (int i = 0; i < random; i++)
            {
                pick = pick->next;
            }
        }
        else
        {
            pick = *ptr_even;
            for (int i = 4; i < random; i++)
            {
                pick = pick->next;
            }
        }
        int t;
        scanf("%d", &t);
        getchar();
        printf("Jack, You are disguised as %s\n", pick->name);
        pick->got = 1;
        jackglobal = pick;
        pick->guilty = -1;
        getchar();
        system("cls");
        NewRound(map, ptr_odd, ptr_even, tunnel);
    }
}