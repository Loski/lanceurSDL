#ifndef DEF_MENU
#define DEF_MENU


#include "include.h"
#include "jeux.h"
#include "fonction_Pendu.h"


#define MIN_MENUY 566
#define MIN_MENUX 431
#define MAX_MENUX 169
#define MAX_MENUY 34

enum{MORPION, DEMINEUR, SNAKE, PONG, PENDU};

bool afficherMenuJeux(int jeu);
bool options(int jeu);
void ecrireTexte(int valeurAEcrire);
void TTF_write (const char mot []);

#endif
