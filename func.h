#ifndef FUNC_H
#define FUNC_H

// the clear command
#ifdef _WIN32
#include <conio.h>
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif

typedef struct sell
{
    char what[10];
    char watch[4];
    char who[3];
    int neighbor[6][2];
} cell;

void SetColor(int ForgC);

typedef struct node
{
    char name[3];
    int played;        // played this round or not
    int watch;         // invisible or visible
    cell *place;       // where are they?
    char ability[100]; // for the user
    struct node *next; // for linked list
} person;

void create_cards(person **ptr_odd, person **ptr_even, cell map[9][13]);

void shuffle(person **ptr_odd, person **ptr_even);
void printfcard(person *odd, person *even);

void SH_play();
void JW_play();
void JS_play();
void IL_play();
void MS_play();
void SG_play();
void WG_play();
void JB_plsy();

void display_map(cell map[9][13]);

void GetDeafultGame(cell map[9][13]);

#endif