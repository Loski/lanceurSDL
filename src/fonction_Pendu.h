#ifndef DEF_PENDU_FCT
#define DEF_PENDU_FCT

#include "include.h"
#include "menu.h"
#include "jeux.h"


char traitementLettre(char lettre);
char* piocherMot(void);
char convertion(const char lettre);
int tailleDico(FILE* dico);
int newMot(const char tab[]);

#endif
