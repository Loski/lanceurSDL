#ifndef JEUX_H_INCLUDED
#define JEUX_H_INCLUDED

#include "include.h"
#define LINUX 1
#ifdef _WIN32
#undef LINUX
#define LINUX 0
#endif
	bool Apropos(void);
	void iniPos(SDL_Rect *pos,Sint16 largeur,Sint16 hauteur);
	int testPos(SDL_Rect pos[], int tailleX, int tailleY, int n,SDL_Event event);
	bool testMenu(SDL_Event event);
	bool play(int choix,int menu);
	void afficher(int nombre,SDL_Surface *jeux[]);
	bool JEUX(void);
	bool Morpion(void);
	bool snake(void);
	bool pendu(bool *rejouer);
	bool pong(void);
	bool demineur(void);
	void verifSetVideo(SDL_Surface *ecran);
	void verifChargImage(SDL_Surface *image);
	void verifChargPolice(TTF_Font *police);
	void verifChargTexte(FILE* texte);
	void init(void);
	void closeSDL(void);
#endif
