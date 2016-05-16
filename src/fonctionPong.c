#include "pong.h"


void blit(SDL_Surface *raquette[],SDL_Rect pos[])
{
    int i;
    SDL_Surface *ecran;
    ecran = SDL_GetVideoSurface();
    for(i=0; i<2; i++)
    {
        SDL_BlitSurface(raquette[i],NULL,ecran,&pos[i]);
    }

}

bool bougerBalle(SDL_Rect raquette[],SDL_Rect balle, Coordonnee *mouvement,int tailleRaquette)
{
    if(balle.y <= 142) /* cas ou la balle touche la barre horizontal haute du pong*/
    {
        mouvement->y = 1;
    }
    else if(balle.y >= 560) /* cas ou la balle la barre horizontal basse du pong*/
    {
        mouvement->y = -1;
    }
    else if (balle.x <=35) // On regarde si il y a colision avec la raquette gauche, sinon l'ordi gagne
    {
        if((colision(raquette,balle,mouvement,tailleRaquette,0)))
            return true;
        else
            mouvement->victoireOrdi = true;
        return false;
    }
    else if(balle.x >= 550)// On regarde si il y a colision avec la raquette droite, sinon le joueur gagne
    {
        if((colision(raquette,balle,mouvement,tailleRaquette,1)))
            return true;
        else
            mouvement->victoireJoueur = true;
        return false;
    }
    return true;
}

void initialiserCoordonnee(Coordonnee *mouvement)
{
    mouvement->x = -1; /* déplacement horizontal*/
    mouvement->y = 1; /* déplacement vertical*/
    mouvement->victoireJoueur = false;
    mouvement->victoireOrdi = false;
    mouvement->vitesseX = (Sint16)((rand() % (MAX - MIN + 1)) + MIN);
    mouvement->vitesseY = (Sint16)((rand() % (MAX - MIN + 1)) + MIN);
}
bool colision(SDL_Rect raquette[],SDL_Rect balle,Coordonnee *mouvement,int tailleRaquette,int i)
{
    if(raquette[i].y <= (balle.y + 20) && (raquette[i].y + tailleRaquette)>=(balle.y))
    {
        /* On regarde si la position de la balle est la même que la position de la raquette, donc si un renvoie peux avoir lieu */
        if(i) /* Cas Ordi*/
        {
            mouvement->x = -1;
            mouvement->vitesseX = (Sint16)((rand() % (MAX - MIN + 1)) + MIN);
            mouvement->vitesseY = (Sint16)((rand() % (MAX - MIN + 1)) + MIN);
        }
        else /* Cas Joueur  */
        {
            mouvement->x = 1;
            mouvement->vitesseX = (Sint16)((rand() % (MAX - MIN + 1)) + MIN);
            mouvement->vitesseY = (Sint16)((rand() % (MAX - MIN + 1)) + MIN);
        }
        return true;
    }
    return false;
}

void affiche_score_pong(int pointO,int pointJ, SDL_Rect posTexte[])
{
    int i;
    char scoreJ[3] = {0},scoreO[3] = {0};
    TTF_Font *police = NULL;
    SDL_Surface *ecran = NULL,*texte[2] = {NULL};
    SDL_Color Noir = {0,0,0,0};
    /***********/
    police = TTF_OpenFont("Jeux/Pong/sources/Call.ttf", 40);
    verifChargPolice(police);
    ecran = SDL_GetVideoSurface();
    verifSetVideo(ecran);
    sprintf(scoreJ,"%d",pointJ);
    sprintf(scoreO,"%d",pointO);
    texte[0] = TTF_RenderText_Blended(police, scoreJ, Noir);
    texte[1] = TTF_RenderText_Blended(police, scoreO, Noir);
    /**********/
    for(i=0; i<2; i++)
    {
        SDL_BlitSurface(texte[i], NULL, ecran, &posTexte[i]);
        SDL_FreeSurface(texte[i]);
    }
    TTF_CloseFont(police);
}
Sint16 getTailleRaquette(void) /* Sert à obtenir la taille de la raquette dans raquette.txt */
{
    FILE *raquette = NULL;
    Sint16 nombre;
    raquette = fopen("Jeux/Pong/sources/raquette.txt","r");
    verifChargTexte(raquette);
    fscanf(raquette,"%hd",&nombre); /* format Sint16 */
    fclose(raquette);
    return nombre;
}

