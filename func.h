#ifndef FUNC_H
#define FUNC_H

typedef struct sell
{
    char what[10];
    char watch[4];
    char who[3];
    struct sell *neighbor[6];
    int x, y;

} cell;

void SetColor(int ForgC);
typedef struct node
{
    char name[3];
    int played; // played this round or not
    int guilty;
    int watch; // invisible or visible
    int got;
    cell *place;       // where are they?
    char ability[100]; // for the user
    struct node *next; // for linked list
} person;
void MainMenu(cell map[9][13], person **ptr_odd, person **ptr_even, cell *tunnel[8], cell *lamp[8]);
void create_cards(person **ptr_odd, person **ptr_even, cell map[9][13]);
void shuffle(person **ptr_odd, person **ptr_even);
void printfcard(person *odd, person *even);
void NewRound(cell map[9][13], person **ptr_odd, person **ptr_even, cell *tunnel[8] ,cell *lamp[8]);
person *FindThePerson(person *odd, person *even, char names[3]);
int CanMove(cell map[9][13], person mover, char *move, cell *tunnel[8], cell **last);
char *GetTheMove();
void SH_play(cell map[9][13], cell *tunnel[8], person *odd, person *even);
void JW_play(cell map[9][13], cell *tunnel[8], person *odd, person *even);
void JS_play(cell map[9][13], cell *tunnel[8], person *odd, person *even, cell *lamp[8]);
void IL_play(cell map[9][13], cell *tunnel[8], person *odd, person *even);
void MS_play(cell map[9][13], cell *tunnel[8], person *odd, person *even);
void SG_play(cell map[9][13], cell *tunnel[8], person *odd, person *even);
void WG_play(cell map[9][13], cell *tunnel[8], person *odd, person *even);
void JB_play(cell map[9][13], cell *tunnel[8], person *odd, person *even);
void display_map(cell map[9][13], person *odd, person *even);
void GetDeafultGame(cell map[9][13]);
#endif