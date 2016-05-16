#ifndef FONCTIONMORPION_H_INCLUDED
#define FONCTIONMORPION_H_INCLUDED

#include "include.h"
#include "menu.h"
#include "jeux.h"


#define MIN 75
#define MAX 100
#define TAILLE 150

typedef struct IMAGE IMAGE;
struct IMAGE
{
    SDL_Surface *fond;
    SDL_Surface *croix;
    SDL_Surface *rond;
};

enum {RIEN,ROND,CROIX};

int verifie(int blocks[]);
bool check(int blocks[], bool *full_cases);
void affiche_score(SDL_Surface *texte[], int manche, int pointJ[], SDL_Rect posTexte[]);
void initialiseImagesMorpion(IMAGE* images);

#endif
