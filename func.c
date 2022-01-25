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
int endglobal = 0;
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
    char c1[2] = " ";
    char *c = c1;
    *c = getchar(); // for \n
    *c = getchar();
    char *des = (char *)malloc(10 * sizeof(char));
    for (int i = 0; i < 10; i++)
    {
        des[i] = '\0';
    }
    while (*c != '\0' || *c != '\r' || *c != '\n')
    {
        if (*c == 'u' || *c == 'i' || *c == 'k' || *c == 'm' || *c == 'n' || *c == 'h' || *c == 'j' || (*c >= '1' && *c <= '8' && des[strlen(des) - 1] == 'j'))
        {
            strcat(des, c);
        }
        else if (*c == ' ' || *c == '\t' || *c == '\v' || *c == '\f')
        {
            ;
        }
        else if (*c == '\n')
        {
            break;
        }
        else
        {
            printf("not a valid command, please try again using(u,i,k,m,n,h,j,1-8)");
            // strcpy(des, ptr);
        }
        *c = getchar();
    }
    int t2 = strlen(des);
    des[t2] = '\0';
    return des;
}

int CanMove(cell map[9][13], person mover, char *move, cell *tunnel[8], cell **last)
{
    printf("%s\n", move);
    char *ptr = move;
    int count = 0;
    cell *where = mover.place;
    int total = 3;
    if (!strcmp(mover.name, "MS"))
    {
        total = 4;
    }
    while (*ptr != '\0' && ptr != "/n")
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
                    return -2;
                }
            }
        }
        else if (*ptr == 'i')
        {
            where = where->neighbor[1];
            if (where->x == -1)
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
            if (where->x == -1)
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
            if (where->x == -1)
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
            if (where->x == -1)
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
            if (where->x == -1)
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
            printf("%d %d", count, total);
            return -15;
        }
    }
    if (!strncmp(where->what, "apartment", 4) || !strncmp(where->what, "lamp", 3))
    {

        return -16;
    }
    if (strcmp(where->who, "NU"))
    {
        return -17;
    }
    if (*last == mover.place)
    {
        return -18;
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

void NewRound(cell map[9][13], person **ptr_odd, person **ptr_even, cell *tunnel[8], cell *lamp[8])
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
        JS_play(map, tunnel, *ptr_odd, *ptr_even, lamp);
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
    else if (!strncmp(chosen, "WG", 2))
    {
        WG_play(map, tunnel, *ptr_odd, *ptr_even);
    }
    else if (!strncmp(chosen, "JB", 2))
    {
        JB_play(map, tunnel, *ptr_odd, *ptr_even);
    }
    system("cls");
    display_map(map, *ptr_odd, *ptr_even);
}

void SH_play(cell map[9][13], cell *tunnel[8], person *odd, person *even)
{
    printf("SH : move 1 to 3 cells : ");
    cell *last;
    person *sherlock = FindThePerson(odd, even, "SH");
    char *m = GetTheMove();
    int can = CanMove(map, *sherlock, m, tunnel, &last);
    while (can != 1)
    {
        printf("\n not a correct move,try again: ");
        m = GetTheMove();
        can = CanMove(map, *sherlock, m, tunnel, &last);
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
        pick->guilty = 1;
        printf("%s is innocent", pick->name);
        return;
    }
}

void JW_play(cell map[9][13], cell *tunnel[8], person *odd, person *even)
{
    return;
}

void JS_play(cell map[9][13], cell *tunnel[8], person *odd, person *even, cell *lamp[8])
{
    person *john = FindThePerson(odd, even, "JS");
    printf("JS : choose: 1)%s           2)move 1 to 3 steps", john->ability);
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {
        printf("\nchoose the lamp you want to turn off(a number) : ");
        int off;
        scanf("%d", &off);
        printf("\nchoose the lamp you want to turn on(a number) : ");
        int on;
        scanf("%d", &on);
        while (off == on)
        {
            printf("lamps must be different");
            printf("\nchoose the lamp you want to turn off(a number) : ");
            scanf("%d", &off);
            printf("\nchoose the lamp you want to turn on(a number) : ");
            scanf("%d", &on);
        }

        if (off == 1)
        {
            strcpy(lamp[0]->what, "lamp1-off");
        }
        else if (off == 2)
        {
            strcpy(lamp[1]->what, "lamp2-off");
        }
        else if (off == 3)
        {
            strcpy(lamp[2]->what, "lamp3-off");
        }
        else if (off == 4)
        {
            strcpy(lamp[3]->what, "lamp4-off");
        }
        else if (off == 5)
        {
            strcpy(lamp[4]->what, "lamp5-off");
        }
        else if (off == 6)
        {
            strcpy(lamp[5]->what, "lamp6-off");
        }
        else if (off == 7)
        {
            strcpy(lamp[6]->what, "lamp7-off");
        }
        else if (off == 8)
        {
            strcpy(lamp[7]->what, "lamp8-on!");
        }
        off = on;
        if (off == 1)
        {
            strcpy(lamp[0]->what, "lamp1-on!");
        }
        else if (off == 2)
        {
            strcpy(lamp[1]->what, "lamp2-on!");
        }
        else if (off == 3)
        {
            strcpy(lamp[2]->what, "lamp3-on!");
        }
        else if (off == 4)
        {
            strcpy(lamp[3]->what, "lamp4-on!");
        }
        else if (off == 5)
        {
            strcpy(lamp[4]->what, "lamp5-on!");
        }
        else if (off == 6)
        {
            strcpy(lamp[5]->what, "lamp6-on!");
        }
        else if (off == 7)
        {
            strcpy(lamp[6]->what, "lamp7-on!");
        }
        else if (off == 8)
        {
            strcpy(lamp[7]->what, "lamp8-on!");
        }

        printf("\nJS : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(map, *john, m, tunnel, &last);
        while (can != 1)
        {
            printf("\n not a correct move,try again: ");
            m = GetTheMove();
            can = CanMove(map, *john, m, tunnel, &last);
            printf("%d", can);
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
        printf("\nJS : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(map, *john, m, tunnel, &last);
        while (can != 1)
        {
            printf("\n not a correct move,try again: ");
            m = GetTheMove();
            can = CanMove(map, *john, m, tunnel, &last);
        }
        if (can == 1)
        {
            strcpy(john->place->who, "NU");
            john->place = last;
            strcpy(last->who, "JS");
            john->played = 1;
        }
        printf("\nchoose the lamp you want to turn off(a number) : ");
        int off;
        scanf("%d", &off);
        printf("\nchoose the lamp you want to turn on(a number) : ");
        int on;
        scanf("%d", &on);
        while (off == on)
        {
            printf("lamps must be different");
            printf("\nchoose the lamp you want to turn off(a number) : ");
            scanf("%d", &off);
            printf("\nchoose the lamp you want to turn on(a number) : ");
            scanf("%d", &on);
        }

        if (off == 1)
        {
            strcpy(lamp[0]->what, "lamp1-off");
        }
        else if (off == 2)
        {
            strcpy(lamp[1]->what, "lamp2-off");
        }
        else if (off == 3)
        {
            strcpy(lamp[2]->what, "lamp3-off");
        }
        else if (off == 4)
        {
            strcpy(lamp[3]->what, "lamp4-off");
        }
        else if (off == 5)
        {
            strcpy(lamp[4]->what, "lamp5-off");
        }
        else if (off == 6)
        {
            strcpy(lamp[5]->what, "lamp6-off");
        }
        else if (off == 7)
        {
            strcpy(lamp[6]->what, "lamp7-off");
        }
        else if (off == 8)
        {
            strcpy(lamp[7]->what, "lamp8-on!");
        }
        off = on;
        if (off == 1)
        {
            strcpy(lamp[0]->what, "lamp1-on!");
        }
        else if (off == 2)
        {
            strcpy(lamp[1]->what, "lamp2-on!");
        }
        else if (off == 3)
        {
            strcpy(lamp[2]->what, "lamp3-on!");
        }
        else if (off == 4)
        {
            strcpy(lamp[3]->what, "lamp4-on!");
        }
        else if (off == 5)
        {
            strcpy(lamp[4]->what, "lamp5-on!");
        }
        else if (off == 6)
        {
            strcpy(lamp[5]->what, "lamp6-on!");
        }
        else if (off == 7)
        {
            strcpy(lamp[6]->what, "lamp7-on!");
        }
        else if (off == 8)
        {
            strcpy(lamp[7]->what, "lamp8-on!");
        }
    }

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
    person *jeremy = FindThePerson(odd, even, "JB");
    printf("JB : choose: 1)%s           2)move 1 to 3 steps", jeremy->ability);
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {
        printf("\nchoose the tunnel you want to close(a number) : ");
        int off;
        scanf("%d", &off);
        printf("\nchoose the tunnel you want to open(a number) : ");
        int on;
        scanf("%d", &on);
        while (off == on)
        {
            printf("tunnels must be different");
            printf("\nchoose the tunnel you want to turn off(a number) : ");
            scanf("%d", &off);
            printf("\nchoose the tunnel you want to turn on(a number) : ");
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

        printf("\nJB : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(map, *jeremy, m, tunnel, &last);
        while (can != 1)
        {
            printf("%d\n", can);
            printf("\n not a correct move,try again: ");
            m = GetTheMove();
            can = CanMove(map, *jeremy, m, tunnel, &last);
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
        printf("\nJB : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(map, *jeremy, m, tunnel, &last);
        while (can != 1)
        {
            printf("\n not a correct move,try again: ");
            m = GetTheMove();
            can = CanMove(map, *jeremy, m, tunnel, &last);
        }
        if (can == 1)
        {
            strcpy(jeremy->place->who, "NU");
            jeremy->place = last;
            strcpy(last->who, "JB");
            jeremy->played = 1;
        }
        printf("\nchoose the tunnel you want to close(a number) : ");
        int off;
        scanf("%d", &off);
        printf("\nchoose the tunnel you want to open(a number) : ");
        int on;
        scanf("%d", &on);
        while (off == on)
        {
            printf("tunnels must be different");
            printf("\nchoose the tunnel you want to turn off(a number) : ");
            scanf("%d", &off);
            printf("\nchoose the tunnel you want to turn on(a number) : ");
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
    display_map(map, odd, even);
    int n;
    scanf("%d", &n);
    return;
}

void MainMenu(cell map[9][13], person **ptr_odd, person **ptr_even, cell *tunnel[8], cell *lamp[8])
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
        NewRound(map, ptr_odd, ptr_even, tunnel, lamp);
    }
}