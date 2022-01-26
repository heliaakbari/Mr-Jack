#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include "func.h"
#include <time.h>
#include <conio.h>
int jackvisibility = 0;
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
        SetColor(yellow);
    else if (!strncmp(str, "side", 4))
        SetColor(black);
    else if (!strncmp(&str[5], "clsd", 3) && !strncmp(str, "tnl", 3))
        SetColor(red);
    else if (!strncmp(&str[5], "open", 3) && !strncmp(str, "tnl", 3))
        SetColor(lightred);
    else if (!strncmp(str, "on!", 3))
        SetColor(brown);
    else if (!strncmp(str, "off", 3))
        SetColor(black);
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
    if (turnglobal % 8 == 1 || turnglobal % 8 == 4 || turnglobal % 8 == 6 || turnglobal % 8 == 7)
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
    if (roundglobal == 1 && turnglobal == 1)
    {
        printf("  Jack is visible");
    }
    else
    {
        if (jackglobal->watch == 0)
        {
            printf("  Jack is invisible");
        }
        else
        {
            printf("  Jack is visible");
        }
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
    strcpy(tptr->ability, "identify an innocent");
    tptr->next = (person *)malloc(sizeof(person));
    tptr = tptr->next;
    strncpy(tptr->name, "JW", 3);
    tptr->played = 0;
    tptr->got = 0;
    tptr->guilty = 0;
    tptr->watch = 0;
    tptr->place = &map[5][0];
    strcpy(tptr->ability, "iluminate 1 direction");
    tptr->next = (person *)malloc(sizeof(person));
    tptr = tptr->next;
    strncpy(tptr->name, "JS", 3);
    tptr->played = 0;
    tptr->got = 0;
    tptr->guilty = 0;
    tptr->watch = 1;
    tptr->place = &map[3][6];
    strcpy(tptr->ability, "turn off 1 lamp and turn on another");
    tptr->next = (person *)malloc(sizeof(person));
    tptr = tptr->next;
    strncpy(tptr->name, "IL", 3);
    tptr->played = 0;
    tptr->got = 0;
    tptr->guilty = 0;
    tptr->watch = 1;
    tptr->place = &map[5][4];
    strcpy(tptr->ability, "open 1 exit and close another");
    tptr->next = NULL;
    even = (person *)malloc(sizeof(person));
    tptr = even;
    strncpy(tptr->name, "MS", 3);
    tptr->played = 0;
    tptr->got = 0;
    tptr->guilty = 0;
    tptr->watch = 0;
    tptr->place = &map[8][8];
    strcpy(tptr->ability, "move through apartments but can't stop in them");
    tptr->next = (person *)malloc(sizeof(person));
    tptr = tptr->next;
    strncpy(tptr->name, "SG", 3);
    tptr->played = 0;
    tptr->watch = 0;
    tptr->guilty = 0;
    tptr->got = 0;
    tptr->place = &map[4][12];
    strcpy(tptr->ability, "call chracter(s) 3 cells closer to himself");
    tptr->next = (person *)malloc(sizeof(person));
    tptr = tptr->next;
    strncpy(tptr->name, "WG", 3);
    tptr->played = 0;
    tptr->guilty = 0;
    tptr->got = 0;
    tptr->watch = 0;
    tptr->place = &map[1][4];
    strcpy(tptr->ability, "change place with another chracter");
    tptr->next = (person *)malloc(sizeof(person));
    tptr = tptr->next;
    strncpy(tptr->name, "JB", 3);
    tptr->played = 0;
    tptr->guilty = 0;
    tptr->got = 0;
    tptr->watch = 1;
    tptr->place = &map[4][8];
    strcpy(tptr->ability, "open 1 tunnel and close another");
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
            printf("not a valid command, please try again using(u,i,k,m,n,h,j,1-8):  ");
            char ptr1[10];
            char *ptr = ptr1;
            ptr = GetTheMove();
            strcpy(des, ptr);
        }
        *c = getchar();
    }
    int t2 = strlen(des);
    while (t2 == 0)
    {
        printf("not a valid command, please try again using(u,i,k,m,n,h,j,1-8) :");
        char ptr1[10];
        char *ptr = ptr1;
        ptr = GetTheMove();
        strcpy(des, ptr);
    }
    t2 = strlen(des);
    des[t2] = '\0';
    return des;
}

int CanMove(person mover, char *move, cell **last)
{
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
void newturn()
{
    system("cls");
    display_map();
    printfcard();
    printf("\nchoose one(write the initials-capital letter-and press enter): ");
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
    printf(" : ");
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

    turnglobal = 1;
    system("cls");
    if (roundglobal % 2 == 1)
    {
        shuffle();
    }
    for (int i = 0; i < 4; i++)
    {
        newturn();
        turnglobal++;
        // checkend();
    }
    roundglobal++;
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

void johnlight(cell *place, int direction)
{
    if (place->neighbor[direction] == NULL)
    {
        return;
    }
    cell *ptr = place->neighbor[direction];
    while (ptr->neighbor[direction] != NULL)
    {
        strcpy(ptr->watch, "on!");
        ptr = ptr->neighbor[direction];
    }
    strcpy(ptr->watch, "on!");
    return;
}

void johnlight_off(cell *place, int direction)
{
    cell *ptr = place->neighbor[direction];
    while (ptr->neighbor[direction] != NULL)
    {
        strcpy(ptr->watch, "off");
        printf("done");
        ptr = ptr->neighbor[direction];
    }
    strcpy(ptr->watch, "off");
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
        printf("\n not a correct move,try again: ");
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
        printf("        %s is innocent", pick->name);
        getchar();
        return;
    }
}

void JW_play()
{
    person *john = FindThePerson("JW");
    printf("\nJW : move 1 to 3 cells : ");
    cell *last;
    char *m = GetTheMove();
    int can = CanMove(*john, m, &last);
    while (can != 1)
    {
        printf("\n not a correct move,try again: ");
        m = GetTheMove();
        can = CanMove(*john, m, &last);
        printf("%d", can);
    }
    if (can == 1)
    {
        strcpy(john->place->who, "NU");
        john->place = last;
        strcpy(last->who, "JW");
        john->played = 1;
    }

    printf("\nchoose the direction you want to lighten(i,u,k,m,n,h):      ");
    char move;
    scanf(" %c", &move);
    johnlight_off(john->place, johndirection);
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
    for (int i = 0; i < 8; i++)
    {
        if (!strncmp(&(lamp[i]->what[6]), "on!", 3))
        {
            for (int j = 0; j < 6; j++)
            {
                if (lamp[i]->neighbor[j] == NULL)
                {
                    continue;
                }
                strcpy(lamp[i]->neighbor[j]->watch, "on!");
            }
        }
        else if (!strncmp(&(lamp[i]->what[6]), "off", 3))
        {
            for (int j = 0; j < 6; j++)
            {
                if (lamp[i]->neighbor[j] == NULL)
                {
                    continue;
                }
                strcpy(lamp[i]->neighbor[j]->watch, "off");
            }
        }
    }
    return;
}

void JS_play()
{
    person *john = FindThePerson("JS");
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
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[0]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 2)
        {
            strcpy(lamp[1]->what, "lamp2-off");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[1]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 3)
        {
            strcpy(lamp[2]->what, "lamp3-off");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[2]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 4)
        {
            strcpy(lamp[3]->what, "lamp4-off");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[3]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 5)
        {
            strcpy(lamp[4]->what, "lamp5-off");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[4]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 6)
        {
            strcpy(lamp[5]->what, "lamp6-off");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[6]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 7)
        {
            strcpy(lamp[6]->what, "lamp7-off");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[6]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 8)
        {
            strcpy(lamp[7]->what, "lamp8-on!");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[7]->neighbor[i]->watch, "off");
            }
        }
        off = on;
        if (off == 1)
        {
            strcpy(lamp[0]->what, "lamp1-on!");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[0]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 2)
        {
            strcpy(lamp[1]->what, "lamp2-on!");
            for (int i = 0; i < 6; i++)
            {
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

        printf("\nJS : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*john, m, &last);
        while (can != 1)
        {
            printf("\n not a correct move,try again: ");
            m = GetTheMove();
            can = CanMove(*john, m, &last);
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
        int can = CanMove(*john, m, &last);
        while (can != 1)
        {
            printf("\n not a correct move,try again: ");
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
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[0]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 2)
        {
            strcpy(lamp[1]->what, "lamp2-off");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[1]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 3)
        {
            strcpy(lamp[2]->what, "lamp3-off");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[2]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 4)
        {
            strcpy(lamp[3]->what, "lamp4-off");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[3]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 5)
        {
            strcpy(lamp[4]->what, "lamp5-off");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[4]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 6)
        {
            strcpy(lamp[5]->what, "lamp6-off");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[6]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 7)
        {
            strcpy(lamp[6]->what, "lamp7-off");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[6]->neighbor[i]->watch, "off");
            }
        }
        else if (off == 8)
        {
            strcpy(lamp[7]->what, "lamp8-on!");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[7]->neighbor[i]->watch, "off");
            }
        }
        off = on;
        if (off == 1)
        {
            strcpy(lamp[0]->what, "lamp1-on!");
            for (int i = 0; i < 6; i++)
            {
                strcpy(lamp[0]->neighbor[i]->watch, "on!");
            }
        }
        else if (off == 2)
        {
            strcpy(lamp[1]->what, "lamp2-on!");
            for (int i = 0; i < 6; i++)
            {
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
    printf("SG : choose: 1)call person(s) 3 cells closer           2)move 1 to 3 steps");
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {

        for (int i = 0; i < 3; i++)
        {
            printf("\ninsert the person's initials:     ");
            char close[3];
            char *ptr = close;
            scanf(" %s", close);
            person *called = FindThePerson(ptr);
            char move;
            char themove[2] = " ";
            if (called->place->y == goodley->place->y && called->place->x > goodley->place->x)
            {
                printf("allowed moves(choose one): u :       ");
                scanf(" %c", &move);
                while (move != 'u')
                {
                    getchar();
                    printf("\n invalid choice, try again:   ");
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
                    printf("\n invalid choice, try again:   ");
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
                    printf("\n invalid choice, try again:   ");
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
                    printf("\n invalid choice, try again:   ");
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
                    printf("\n invalid choice, try again:   ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            cell *last;
            int can = CanMove(*called, &(themove[0]), &last);
            while (can != 1)
            {
                printf("\n not a correct move,try again: ");
                char *m = GetTheMove();
                while (strlen(m) != 1)
                {
                    printf("\n not a correct move,try again: ");
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

        printf("\nSG : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*goodley, m, &last);
        while (can != 1)
        {
            printf("\n not a correct move,try again: ");
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
        printf("\nSG : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*goodley, m, &last);
        while (can != 1)
        {
            printf("\n not a correct move,try again: ");
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
            printf("\ninsert the person's initials:     ");
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
                    printf("\n invalid choice, try again:   ");
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
                    printf("\n invalid choice, try again:   ");
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
                    printf("\n invalid choice, try again:   ");
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
                    printf("\n invalid choice, try again:   ");
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
                    printf("\n invalid choice, try again:   ");
                    scanf(" %c", &move);
                }
                themove[0] = move;
            }
            cell *last;
            int can = CanMove(*called, &(themove[0]), &last);
            while (can != 1)
            {
                printf("\n not a correct move,try again: ");
                char *m = GetTheMove();
                while (strlen(m) != 1)
                {
                    printf("\n not a correct move,try again: ");
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
    printf("IL : choose: 1)%s           2)move 1 to 3 steps", lesterade->ability);
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {
        printf("\nchoose the exit you want to close(1-4) : ");
        int off;
        scanf("%d", &off);
        printf("\nchoose the exit you want to open(1-4) : ");
        int on;
        scanf("%d", &on);
        while (off == on)
        {
            printf("lamps must be different");
            printf("\nchoose the exit you want to close(1-4) : ");
            scanf("%d", &off);
            printf("\nchoose the exit you want to close(1-4) : ");
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
        printf("\nIL : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*lesterade, m, &last);
        while (can != 1)
        {
            printf("\n not a correct move,try again: ");
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
        printf("\nIL : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*lesterade, m, &last);
        while (can != 1)
        {
            printf("\n not a correct move,try again: ");
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
        printf("\nchoose the exit you want to close(1-4) : ");
        int off;
        scanf("%d", &off);
        printf("\nchoose the exit you want to open(1-4) : ");
        int on;
        scanf("%d", &on);
        while (off == on)
        {
            printf("lamps must be different");
            printf("\nchoose the exit you want to close(1-4) : ");
            scanf("%d", &off);
            printf("\nchoose the exit you want to close(1-4) : ");
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
    printf("\nMS : move 1 to 4 cells (you can move through everything): ");
    cell *last;
    char *m = GetTheMove();
    int can = CanMove(*stealthy, m, &last);
    while (can != 1)
    {
        printf("\n not a correct move,try again: ");
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
    printf("WG : choose: 1)change places with someone    *or*     2)move 1 to 3 steps");
    int choice;
    scanf("%d", &choice);
    if (choice == 1)
    {
        printf("/n please insert the person's initials you want to change places with:      ");
        char select[3];
        char *ptr = select;
        scanf(" %s", ptr);
        person *changewith = FindThePerson(ptr);
        cell *temp = wili->place;
        wili->place = changewith->place;
        wili->played = 1;
        changewith->place = temp;
        strcpy(changewith->place->who, ptr);
        strcpy(wili->place->who, "WG");
    }
    else if (choice == 2)
    {
        printf("\nWG : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*wili, m, &last);
        while (can != 1)
        {
            printf("\n not a correct move,try again: ");
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
        int can = CanMove(*jeremy, m, &last);
        while (can != 1)
        {
            printf("%d\n", can);
            printf("\n not a correct move,try again: ");
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
        printf("\nJB : move 1 to 3 cells : ");
        cell *last;
        char *m = GetTheMove();
        int can = CanMove(*jeremy, m, &last);
        while (can != 1)
        {
            printf("\n not a correct move,try again: ");
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
    return;
}

void MainMenu()
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
        create_cards();
        shuffle();
        GetDeafultGame();
        system("cls");
        printf("It is Jack's turn. Jack please press 1 to see your identity\n");
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
        printf("Jack, You are disguised as %s\n", pick->name);
        pick->got = 1;
        jackglobal = pick;
        getchar();
        system("cls");
        NewRound();
    }
}