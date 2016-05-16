#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

#include "include.h"
#include "menu.h"
#include "jeux.h"


typedef struct IMAGE IMAGE;
struct IMAGE
{
    SDL_Surface *fond;
    SDL_Surface *tete;
    SDL_Surface *corps;
    SDL_Surface *pomme;
};

typedef struct POS POS;
struct POS
{
    int x;
    int y;
};

enum {BAS,GAUCHE,HAUT,DROITE};

void initialiseGrille(int valeurCases[22][17],POS *tete, POS *fin);
void afficheGrille(IMAGE images, int valeurCases[22][17],SDL_Rect posBlock);
void afficheScore(int score);
void placePomme(int valeurCases[22][17]);
int deplaceSnake(int direction, POS* tete, POS* fin, int valeurCases[22][17]); /* -2 : corps // -1 : mur // 1 pomme // 0 rien */
void initialiseImagesSnake(IMAGE* images);

#endif
