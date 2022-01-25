#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>
/*
typedef struct sell
{
    char what[10];
    char watch[4];
    char who[3];
    int neighbor[6][2];
} cell;

typedef struct node
{
    char name[3];
    int played;        // played this round or not
    int watch;         // invisible or visible
    cell *place;       // where are they?
    char ability[100]; // for the user
    struct node *next; // for linked list
} person;

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



int main()
{
    cell map[9][13];
    GetDeafultGame(map);
    person *odd, *even;
    create_cards(&odd, &even, map);
    for (int i = 0; i < 5; i++)
    {
        shuffle(&odd, &even);
        printfcard(odd, even);
        printf("\n");
    }
    return 0;
}
*/
int main()
{
    char des[] = "absd";
    printf("%d", strlen(des));
}