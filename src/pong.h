#ifndef DEF_PONG
#define DEF_PONG
#define MAX 20
#define MIN 10
#define TAILLE_BALLE 20
#define MAX_BALLE 22

#include "include.h"
#include "menu.h"
#include "jeux.h"
#include <stdint.h>

typedef struct Coordonnee Coordonnee;
struct Coordonnee
{
    uint16_t x; // coefficient abcisse balle 
    uint16_t y; // coefficient ordonné balle
    uint16_t vitesseX; // vitesse abcisse
    uint16_t vitesseY; // vitesse ordonné
    bool victoireJoueur;
    bool victoireOrdi;
};

bool bougerBalle(SDL_Rect raquette[],SDL_Rect balle, Coordonnee *mouvement,int tailleraquette);
void blit(SDL_Surface *raquette[],SDL_Rect pos[]);
void initialiserCoordonnee(Coordonnee *mouvement);
bool colision(SDL_Rect raquette[],SDL_Rect balle,Coordonnee *mouvement,int tailleRaquette,int i);
void affiche_score_pong(int pointO,int pointJ, SDL_Rect posTexte[]);
Sint16 getTailleRaquette(void);

#endif
