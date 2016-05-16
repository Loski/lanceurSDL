#include "snake.h"

void initialiseGrille(int valeurCases[22][17], POS *tete, POS *fin)
{
    int i,j;
    fin->x = 7;
    fin->y = 7;
    tete->x = 11;
    tete->y = 7;

    for(i=0; i<22; i++)
    {
        for(j=0; j<17; j++)
        {
            valeurCases[i][j] = 0;
        }
    }
    for(i=0; i<4; i++)
    {
        valeurCases[7+i][7] = 3; /* droite */
    }
    valeurCases[tete->x][tete->y] = 2;
}

void afficheGrille(IMAGE images, int valeurCases[22][17],SDL_Rect posBlock)
{
    int i,j;
    SDL_Surface *screen=NULL;
    screen = SDL_GetVideoSurface();
    verifSetVideo(screen);
    iniPos(&posBlock,0,0);
    SDL_BlitSurface(images.fond,NULL,screen, &posBlock);

    for(i=0; i<22; i++)
    {
        for(j=0; j<17; j++)
        {
            posBlock.x = (Sint16)(25 + (i*25));
            posBlock.y = (Sint16)(125 + (j*25));

            if(valeurCases[i][j] == 1)
            {
                SDL_BlitSurface(images.pomme,NULL,screen, &posBlock);
            }
            else if(valeurCases[i][j] == 2)
            {
                SDL_BlitSurface(images.tete,NULL,screen, &posBlock);
            }
            else if(valeurCases[i][j] > 2)
            {
                SDL_BlitSurface(images.corps,NULL,screen, &posBlock);
            }
        }
    }
}

void afficheScore(int score)
{
    SDL_Rect posTexte;
    SDL_Surface *screen=NULL, *texte=NULL;
    TTF_Font *police = NULL;
    char texteScore[4] = ""; /* inf a 400*/
    SDL_Color colorT = {61, 45, 25, 0};
  /*  posTexte.x = 98;
    posTexte.y = 551;*/
    iniPos(&posTexte,98,551);
    screen = SDL_GetVideoSurface();
    verifSetVideo(screen);
    police = TTF_OpenFont("Jeux/Snake/sources/TEMPSITC.TTF", 25);
    verifChargPolice(police);

    sprintf(texteScore, "%d", score);
    texte = TTF_RenderText_Blended(police, texteScore, colorT);
    SDL_BlitSurface(texte, NULL, screen, &posTexte);
    TTF_CloseFont(police);
}


void placePomme(int valeurCases[22][17])
{
    int aleatoireX, aleatoireY;
    do
    {
        aleatoireX = rand()%22;
        aleatoireY = rand()%17;
    }
    while(valeurCases[aleatoireX][aleatoireY] != 0);
    valeurCases[aleatoireX][aleatoireY] = 1;
}

int deplaceSnake(int direction, POS* tete, POS* fin, int valeurCases[22][17])
{
    /*MUR*/
    switch(direction)
    {
    case HAUT:
        if(tete->y == 0)
            return -1;
        valeurCases[tete->x][tete->y] = 4;
        tete->y--;
        break;

    case GAUCHE:
        if(tete->x == 0)
            return -1;
        valeurCases[tete->x][tete->y] = 5;
        tete->x--;
        break;

    case BAS:
        if(tete->y == 16)
            return -1;
        valeurCases[tete->x][tete->y] = 6;
        tete->y++;
        break;

    case DROITE:
        if(tete->x == 21)
            return -1;
        valeurCases[tete->x][tete->y] = 3;
        tete->x++;
        break;
    default:
        break;
    }
    /* Corps (sauf queue) */
    if(valeurCases[tete->x][tete->y] > 2 && !(tete->x == fin->x && tete->y == fin->y))
        return -2;
    /* pomme*/
    if(valeurCases[tete->x][tete->y] == 1)
    {
        valeurCases[tete->x][tete->y] = 2;
        return 1;
    }

    switch(valeurCases[fin->x][fin->y])
    {
    case 4: /* HAUT */
        valeurCases[fin->x][fin->y]=0;
        fin->y--;
        break;

    case 3: /* DROITE */
        valeurCases[fin->x][fin->y]=0;
        fin->x++;
        break;

    case 6: /* BAS */
        valeurCases[fin->x][fin->y]=0;
        fin->y++;
        break;

    case 5: /* GAUCHE */
        valeurCases[fin->x][fin->y]=0;
        fin->x--;
        break;
    default:
        break;
    }

    valeurCases[tete->x][tete->y] = 2;
    return 0;
}

void initialiseImagesSnake(IMAGE* images)
{
    char LoadIMG[50] = "";
    sprintf(LoadIMG,"Jeux/Snake/sources/fondSnake.png");
    (*images).fond = IMG_Load(LoadIMG);
    sprintf(LoadIMG,"Jeux/Snake/sources/teteSnake.png");
    (*images).tete = IMG_Load(LoadIMG);
    sprintf(LoadIMG,"Jeux/Snake/sources/corpsSnake.png");
    (*images).corps = IMG_Load(LoadIMG);
    sprintf(LoadIMG,"Jeux/Snake/sources/pommeSnake.png");
    (*images).pomme = IMG_Load(LoadIMG);
}

