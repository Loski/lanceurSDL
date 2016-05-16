#include "fonctionMorpion.h"

int verifie(int blocks[])
{
    int i,j,k,valeur = ROND;
    for (k=0; k<2; k++)
    {
        for(i=0; i<2; i++)
        {
            for(j=0; j<3; j++)
            {
                /* ligne */
                if(blocks[i+(3*j)] == valeur && blocks[i+(3*j)+1]==valeur)
                {
                    if(i==0 && blocks[(3*j)+2]== RIEN)
                        return ((3*j)+2);
                    else if(i != 0 && blocks[3*j]== RIEN)
                        return (3*j);
                }
                else if(i == 0 && blocks[(3*j)] == valeur && blocks[(3*j)+2]==valeur)
                {
                    if(blocks[3*j+1]== RIEN)
                        return (3*j+1);
                }
                /* colonne */
                else if(blocks[j+(3*i)] == valeur && blocks[j+(3*(i+1))]==valeur)
                {
                    if(i==0 && blocks[j+(3*2)]== RIEN)
                        return (j+(3*2));
                    else if(i!=0 && blocks[j]== RIEN)
                        return (j);
                }
                else if(i == 0 && blocks[j] == valeur && blocks[j+(3*2)] == valeur)
                {
                    if(blocks[3+j]== RIEN)
                        return (j+3);
                }
            }
        }
        /* diagonale */
        if(blocks[0] == valeur && blocks[4] == valeur && blocks[8] == RIEN)
            return (8);
        else if(blocks[0] == valeur && blocks[8] == valeur && blocks[4] == RIEN)
            return (4);
        else if(blocks[4] == valeur && blocks[8] == valeur && blocks[0] == RIEN)
            return (0);

        if(blocks[2] == valeur && blocks[4] == valeur && blocks[6] == RIEN)
            return (6);
        else if(blocks[2] == valeur && blocks[6] == valeur && blocks[4] == RIEN)
            return (4);
        else if(blocks[4] == valeur && blocks[6] == valeur && blocks[2] == RIEN)
            return (2);

        valeur = CROIX;
    }
    return -1;
}


/******************************************************************************/


bool check(int blocks[],bool *full_case)
{
    int nombre=0,i;

    /***********/
    for(i=0; i<9; i++)
    {
        if(blocks[i]!=RIEN)
            nombre++;
    }
    if(nombre==9)
    {
        *full_case = true;
        return true;
    }

    /***********/

    for(i=0; i<3; i++) /*barre horizontale*/
    {
        if(blocks[0+(3*i)] == CROIX && blocks[1+(3*i)] == CROIX && blocks[2+(3*i)] == CROIX)
            return true;
        else if(blocks[0+(3*i)] == ROND && blocks[1+(3*i)] == ROND && blocks[2+(3*i)] == ROND)
            return true;
    }
    for(i=0; i<3; i++) /* barre verticale */
    {
        if(blocks[0+i] == CROIX && blocks[3+i] == CROIX && blocks[6+i] == CROIX)
            return true;
        else if (blocks[0+i] == ROND && blocks[3+i] == ROND && blocks[6+i] == ROND)
            return true;
    }
    if(blocks[0] == CROIX && blocks[4] == CROIX && blocks[8] == CROIX)
        return true;
    else if(blocks[0] == ROND && blocks[4] == ROND && blocks[8] == ROND)
        return true;

    if(blocks[2] == CROIX && blocks[4] == CROIX && blocks[6] == CROIX)
        return true;
    else if(blocks[2] == ROND && blocks[4] == ROND && blocks[6] == ROND)
        return true;
    /**********/

    return false;
}


/******************************************************************************/


void affiche_score(SDL_Surface *texte[], int manche, int pointJ[], SDL_Rect posTexte[])
{
    int i;
    char score[20] = "";
    TTF_Font *police = NULL;
    SDL_Surface *ecran = NULL;
    SDL_Color Noir = {0, 0, 0,0};
    /***********/
    ecran = SDL_GetVideoSurface();
    police = TTF_OpenFont("Jeux/Morpion/sources/times.ttf", 20);
    verifChargPolice(police);
    verifSetVideo(ecran);

    for(i=0; i<2; i++)
    {
        sprintf(score, "%d points", pointJ[i]);
        texte[i] = TTF_RenderText_Blended(police, score, Noir);
    }

    sprintf(score, "%d", manche);
    texte[2] = TTF_RenderText_Blended(police, score, Noir);
    /**********/
    for(i=0; i<3; i++)
    {
        SDL_BlitSurface(texte[i], NULL, ecran, &posTexte[i]);
        SDL_FreeSurface(texte[i]);
    }

    TTF_CloseFont(police);
}

void initialiseImagesMorpion(IMAGE* images)
{
    char LoadIMG[60] = "";
    sprintf(LoadIMG,"Jeux/Morpion/sources/Morpion_fond.png");
    images->fond = IMG_Load(LoadIMG);
    sprintf(LoadIMG,"Jeux/Morpion/sources/rond.png");
    images->rond = IMG_Load(LoadIMG);
    sprintf(LoadIMG,"Jeux/Morpion/sources/croix.png");
    images->croix = IMG_Load(LoadIMG);
}
