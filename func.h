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

extern int turnglobal;

extern int roundglobal;

extern int endglobal;

extern person *jackglobal;

extern person *odd;

extern person *even;

extern cell map[9][13];
extern cell * out[4];
extern cell *tunnel[8];
extern cell *lamp[8];
void SetColor(int ForgC);

void MainMenu();
void create_cards();
void shuffle();
void printfcard();
void NewRound();
person *FindThePerson(char names[3]);
int CanMove(person mover, char *move, cell **last);
char *GetTheMove();
cell *checkNear(cell *where);
void SH_play();
void JW_play();
void JS_play();
void IL_play();
void MS_play();
void SG_play();
void WG_play();
void JB_play();
void display_map();
void GetDeafultGame();
#endif