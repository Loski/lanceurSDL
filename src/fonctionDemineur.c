#include "include.h"
#include "jeux.h"
#include "demineur.h"

void afficheGrilleDemineur(IMAGE images, int nombreCases,int valeurCases[],bool blockVisible[],int tailleCases, bool drapeau[], int niveau)
{
    char nombre[2];
    int i, posXinit,posYinit;
    SDL_Surface *screen = NULL, *texte = NULL;
    TTF_Font *police = NULL;
    SDL_Color ColorTexte = {0,0,0,0};
    SDL_Rect posFond, posTexte, pos;
    screen = SDL_GetVideoSurface();
    verifSetVideo(screen);
    if(niveau == MOYEN)
    {
        police = TTF_OpenFont("Jeux/Demineur/sources/handwriting-draft_free-version.ttf", 38);
        posXinit = 107;
        posYinit = 134;
    }
    else
    {
        police = TTF_OpenFont("Jeux/Demineur/sources/handwriting-draft_free-version.ttf", 18);
        posXinit = 105;
        posYinit = 132;
    }


    iniPos(&posFond,0,0);
    verifChargPolice(police);
    SDL_BlitSurface(images.fond, NULL,screen, &posFond);

    for(i=0; i<nombreCases; i++)
    {
        posTexte.x = (Sint16)(posXinit + (tailleCases * (i%((int)sqrt(nombreCases)))));
        posTexte.y = (Sint16)(posYinit + (tailleCases * (i/((int)sqrt(nombreCases)))));

        pos.x = (Sint16)(100 + (tailleCases * (i%((int)sqrt(nombreCases)))));
        pos.y = (Sint16)(130 + (tailleCases * (i/((int)sqrt(nombreCases)))));

        if(blockVisible[i] == true)
        {
            if(valeurCases[i] == -1)
                SDL_BlitSurface(images.mine,NULL,screen,&pos);
            else
            {
                SDL_BlitSurface(images.vide,NULL,screen,&pos);
                if(valeurCases[i] > 0)
                {
                    sprintf(nombre, "%d", valeurCases[i]);
                    texte = TTF_RenderText_Blended(police, nombre, ColorTexte);
                    SDL_BlitSurface(texte, NULL, screen, &posTexte);
                }
            }

        }
        else
        {
            if(drapeau[i] == true)
                SDL_BlitSurface(images.drapeau,NULL,screen,&pos);
            else
                SDL_BlitSurface(images.carre,NULL,screen,&pos);
        }
    }
    TTF_CloseFont(police);
}

void afficheScoreDemineur(char difficulte[], int nombreBombes, int nombreCasesRestante)
{
    int i;
    char nombre[50] = "";
    SDL_Rect posScore[3];
    TTF_Font *policeScore = NULL;
    SDL_Color ColorTexte = {28, 32, 78,0};
    SDL_Surface *screen = NULL, *score[3] = {NULL};
    screen = SDL_GetVideoSurface();
    verifSetVideo(screen);
    iniPos(&posScore[0],172,531);
    iniPos(&posScore[1],253,551);
    iniPos(&posScore[2],183,567);

    policeScore = TTF_OpenFont("Jeux/Demineur/sources/times.ttf", 20);
    verifChargPolice(policeScore);
    sprintf(nombre, "%d", nombreBombes);
    score[0] = TTF_RenderText_Blended(policeScore, nombre, ColorTexte);
    sprintf(nombre, "%d", nombreCasesRestante);
    score[1] = TTF_RenderText_Blended(policeScore, nombre, ColorTexte);
    sprintf(nombre, "%s", difficulte);
    score[2] = TTF_RenderText_Blended(policeScore, nombre, ColorTexte);

    for(i=0;i<3;i++)
    {
        SDL_BlitSurface(score[i], NULL, screen, &posScore[i]);
        SDL_FreeSurface(score[i]);
    }
    TTF_CloseFont(policeScore);

}

void initialiseGrilleDemineur(int nombreBombe, int blocks[],int nombreCases, bool blockVisible[], int valeurCases[], bool drapeau[])
{
    int aleatoire,i;
    int longueurCote = (int)sqrt(nombreCases);

    for(i=0; i<nombreCases; i++)
    {
        blockVisible[i] = false;
        valeurCases[i] = 0; /* -1 mine, 0 rien , 1/2/3/4/5/6/7/8 procheMine */
        drapeau[i] = false;
    }

    for(i=0; i<nombreBombe; i++)
    {
        do
        {
            aleatoire = (rand()%nombreCases);
        }while(blocks[aleatoire] == -1);
        blocks[aleatoire] = -1;

        /* GAUCHE */
        if (aleatoire%longueurCote > 0 && blocks[aleatoire - 1] != -1)
            blocks[aleatoire - 1]++;

        /* DROITE */
        if (aleatoire%longueurCote < (longueurCote-1) && blocks[aleatoire + 1] != -1)
            blocks[aleatoire + 1]++;

        /* HAUT */
        if(aleatoire>(longueurCote-1))
        {

        if(blocks[aleatoire - (longueurCote)] != -1)
            blocks[aleatoire - longueurCote]++;

        if(aleatoire%longueurCote > 0 && blocks[aleatoire - (longueurCote+1)] != -1)
            blocks[aleatoire - (longueurCote+1)]++;

        if(aleatoire%longueurCote < (longueurCote-1) && blocks[aleatoire - (longueurCote-1)] != -1)
            blocks[aleatoire - (longueurCote-1)]++;
        }

        /*  BAS */
        if(aleatoire< nombreCases -longueurCote)
        {
            if(blocks[aleatoire + longueurCote] != -1)
                blocks[aleatoire + longueurCote]++;

            if(aleatoire%longueurCote > 0 && blocks[aleatoire + (longueurCote-1)] != -1)
                blocks[aleatoire + (longueurCote-1)]++;

            if(aleatoire%longueurCote < (longueurCote-1) && blocks[aleatoire + (longueurCote+1)] != -1)
                blocks[aleatoire + (longueurCote+1)]++;
        }
    }
}

void initialiseNiveau(int niveau, int* nombreBombe, int* tailleCases, int* nombreCases, int* nombreCasesRestante, int* nombreDrapeauRestant, char* difficulte, SDL_Rect posBlocks[])
{
    int i;

    if(niveau == MOYEN)
    {
        *nombreDrapeauRestant = *nombreBombe = 10;
        *tailleCases = 40;
        *nombreCasesRestante = *nombreCases = 100;
        sprintf(difficulte,"Moyenne");
    }

    if(niveau == DUR)
    {
        *nombreDrapeauRestant = *nombreBombe = 80;
        *tailleCases = 20;
        *nombreCasesRestante = *nombreCases = 400;
        sprintf(difficulte,"difficile");
    }

    for(i=0; i<(*nombreCases);i++)
    {
        posBlocks[i].x = (Sint16)(100 + (*tailleCases)*(i%((int)sqrt(*nombreCases))));
        posBlocks[i].y = (Sint16)(130 + (*tailleCases)*(i/((int)sqrt(*nombreCases))));
    }
}

void initialiseImagesDemineur(IMAGE* images, int tailleCases)
{
    char LoadIMG[50] = "";
    sprintf(LoadIMG,"Jeux/Demineur/sources/vide%d.png",tailleCases);
    (*images).vide = IMG_Load(LoadIMG);
    sprintf(LoadIMG,"Jeux/Demineur/sources/case%d.png",tailleCases);
    (*images).carre = IMG_Load(LoadIMG);
    sprintf(LoadIMG,"Jeux/Demineur/sources/mine%d.png",tailleCases);
    (*images).mine = IMG_Load(LoadIMG);
    sprintf(LoadIMG,"Jeux/Demineur/sources/drapeau%d.png",tailleCases);
    (*images).drapeau = IMG_Load(LoadIMG);
    verifChargImage(images->mine);
    verifChargImage(images->vide);
    verifChargImage(images->drapeau);
    verifChargImage(images->carre);
}

bool verifieDrapeau(bool drapeau[], int valeurCases[],int nombreCases, int nombreBombes)
{
    int i, j=0;
    for(i=0;i<nombreCases;i++)
    {
        if(valeurCases[i] == -1 && drapeau[i] == true)
        {
            j++;
        }
    }
    if(j == nombreBombes)
        return false;
    return true;
}
