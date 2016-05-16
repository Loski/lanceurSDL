#ifndef DEMINEUR_H_INCLUDED
#define DEMINEUR_H_INCLUDED

#include "include.h"
#include "menu.h"
#include "jeux.h"

typedef struct IMAGE IMAGE;
struct IMAGE
{
    SDL_Surface *fond;
    SDL_Surface *vide;
    SDL_Surface *carre;
    SDL_Surface *mine;
    SDL_Surface *drapeau;
};

enum{MOYEN,DUR};

void afficheGrilleDemineur(IMAGE images, int nombreCases,int valeurCases[],bool blockVisible[],int tailleCases, bool drapeau[],int niveau);
void afficheScoreDemineur(char difficulte[], int nombreBombes, int nombreCasesRestante);
void initialiseGrilleDemineur(int nombreBombe, int blocks[],int nombreCases, bool blockVisible[], int valeurCases[], bool drapeau[]);
void initialiseNiveau(int niveau, int* nombreBombe, int* tailleCases, int* nombreCases,int* nombreCasesRestante, int* nombreDrapeauRestant, char* difficulte, SDL_Rect posBlocks[]);
void initialiseImagesDemineur(IMAGE* images, int tailleCases);

bool verifieDrapeau(bool drapeau[], int valeurCases[],int nombreCases, int nombreBombes);

#endif
