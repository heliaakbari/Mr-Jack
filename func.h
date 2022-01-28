#ifndef FUNC_H
#define FUNC_H

typedef struct sell
{
    int x, y;
    char what[10];
    char watch[4];
    char who[3];
    struct sell *neighbor[6];

} cell;

typedef struct node
{
    char name[3];
    int played;        // played this round or not
    int guilty;        //
    int watch;         // invisible or visible
    int got;           // picked or not
    cell *place;       // where are they
    struct node *next; // for linked list
} person;

extern int turnglobal;

extern int roundglobal;

extern int endglobal;

extern person *jackglobal;

extern int johndirection;

extern int jackvisibility;

extern person *odd;

extern person *even;

extern cell map[9][13];
extern cell *out[4];
extern cell *tunnel[8];
extern cell *lamp[8];
void SetColor(int ForgC);
void paint(char *str);
void johnlight(cell *place, int direction);
void newturn();
void MainMenu();
void savethegame();
void create_cards();
void checkend();
void shuffle();
void win();
void loose();
void resumethegame();
void printfcard();
void NewRound();
void checkvisibility();
person *FindThePerson(char names[3]);
int CanMove(person mover, char *move, cell **last);
char *GetTheMove();
void savethegame();
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