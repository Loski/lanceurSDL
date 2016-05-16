#include "pong.h"

bool pong(void)
{

    SDL_Surface *raquette[2]= {NULL},*fond = NULL,*ecran = NULL,*balle=NULL;
    SDL_Rect posRaquette[2], posFond, posTexte[2],posBalle;
    SDL_Event event_pong;
    Sint16 posYMax,tailleRaquette=1,preDefRaquette=0;
    const Sint16 deplacementObligatoire = 10;
    Uint32 tempsPrecedent = 0, tempsActuel = 0;
    int i,pointJ= 0,pointO = 0;
    bool continuer = true,rejouer = true,quitter = true;
    Coordonnee mouvement;
    const int jeu = PONG;

    /*********************************************************************/
    for(i=0; i<2; ++i)
    {
        posTexte[i].x = (Sint16)(105 + i*385);
        posTexte[i].y = 50;
    }
    iniPos(&posFond,0,0);
    SDL_WM_SetCaption("Pong", NULL);
    ecran = SDL_GetVideoSurface();
    verifSetVideo(ecran);
    balle = IMG_Load("Jeux/Pong/sources/balle.png");
    fond = IMG_Load("Jeux/Pong/sources/fondpong.png");
    verifChargImage(balle);
    verifChargImage(fond);
    SDL_BlitSurface(fond,NULL,ecran,&posFond);

    tailleRaquette = getTailleRaquette();/* On recupère la taille de la raquette stocké dans raquette.txt */
    posYMax = (Sint16)(600-(tailleRaquette + MAX_BALLE)); /* taille de l'ecran - la taille de la raquette - la taille du cours en bas */

    do
    {
        initialiserCoordonnee(&mouvement);
        if(preDefRaquette != tailleRaquette) /* Initialise puis permet de modifier la taille de la raquette en plein jeu */
        {
            /* Peut être mit dans la boucle continuer, pour recommencer directement une nouvelle partie quand on change la taille de la raquette via le menu ou une condition continuer = (prpreDefRaquette != tailleRaquette) ? true:false; */
            pointJ = pointO = 0;
            posYMax = (Sint16)(600-(tailleRaquette + MAX_BALLE));
            preDefRaquette = tailleRaquette;
            switch(preDefRaquette) /* On affiche l'image correspondant à la taille stocké dans raquette.txt */
            {
            case 60:
                raquette[0] = raquette[1] = IMG_Load("Jeux/Pong/sources/raquette60.png");
                break;
            case 100:
                raquette[0] = raquette [1] = IMG_Load("Jeux/Pong/sources/raquette100.png");
                break;
            case 170:
                raquette[0] = raquette [1] = IMG_Load("Jeux/Pong/sources/raquette170.png");
                break;
            default:
                raquette[0] = raquette [1] = IMG_Load("Jeux/Pong/sources/raquette100.png");
                preDefRaquette = tailleRaquette = 100;
                break;
            }
            verifChargImage(raquette[0]);
        }
        for(i=0; i<2; i++)
        {
            iniPos(&posRaquette[i],(Sint16)(15 + 550*i),(Sint16)(143+(435/2)-preDefRaquette/2)); /* On met les raquettes au milieu de l'écran réservé au pong */
        }
        iniPos(&posBalle,300,320);
        blit(raquette,posRaquette);
        while(continuer)
        {
            SDL_Flip(ecran);
            SDL_PollEvent(&event_pong);
            switch(event_pong.type)
            {
            case SDL_QUIT:
                rejouer = false;
                continuer = false;
                quitter = false;
                break;
            case SDL_KEYDOWN:
                switch(event_pong.key.keysym.sym)
                {
                case SDLK_ESCAPE: /* MENU */
                    if(afficherMenuJeux(jeu))
                    {
                        continuer = false;
                        rejouer = false;
                        continue; /* Sert à sortir de la boucle continuer */
                    }
                    else
                    {
                        tailleRaquette = getTailleRaquette(); /* On vérifie qu'auncune option n'a été modifié*/
                        event_pong.type = SDLK_1;
                    }
                    break;
                case SDLK_UP: /* Flèche haut */
                    if(posRaquette[0].y> 142)/* On vérifie que la position de la raquette ne dépasse pas les limites min */
                        posRaquette[0].y = (Sint16)(posRaquette[0].y - deplacementObligatoire);/*On la replace*/
                    break;
                case SDLK_DOWN:
                    if(posRaquette[0].y < posYMax)/* On vérifie que la position de la raquette ne dépasse pas les limites max */
                        posRaquette[0].y = (Sint16)(posRaquette[0].y + deplacementObligatoire); /* On la replace */
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            tempsActuel = SDL_GetTicks();
            SDL_BlitSurface(fond,NULL,ecran,&posFond);
            if((tempsActuel - tempsPrecedent) > 25)
            {
                continuer = bougerBalle(posRaquette,posBalle,&mouvement,preDefRaquette); /* Si false, un des deux joueurs a gagné, on sort donc de la boucle */
                posRaquette[1].y = (Sint16)((10*mouvement.y)+posRaquette[1].y); /* On calque les mouvements de la raquette Ordi à ceux de la balle enfin d'avoir une IA basique */
                if(posRaquette[1].y<142)
                {
                    posRaquette[1].y = 142;
                }
                else if(posRaquette[1].y > posYMax)
                {
                    posRaquette[1].y = posYMax;
                }

                posBalle.y = (Sint16)(posBalle.y+(mouvement.vitesseX*mouvement.y));
                posBalle.x = (Sint16)(posBalle.x+(mouvement.vitesseY*mouvement.x));
                tempsPrecedent = tempsActuel;

            }
            else
                {
                    SDL_Delay(25 - (tempsActuel - tempsPrecedent));
                }
            SDL_BlitSurface(balle,NULL,ecran,&posBalle);
            blit(raquette,posRaquette);
            affiche_score_pong(pointO,pointJ,posTexte);

            if(mouvement.victoireJoueur)
            {
                pointJ++;
                continuer=false;
            }
            else if(mouvement.victoireOrdi)
            {
                pointO++;
                continuer = false;
            }
        }
        if(rejouer)
            {
                continuer = true;
                SDL_Delay(1500);
            }
    }
    while(rejouer);
    /* On libère tous */
    SDL_FreeSurface(fond);
    SDL_FreeSurface(balle);
    SDL_FreeSurface(raquette[0]); /* Corre raquete[0] = raquette [1] , deux pointeurs pointent la même image */
    return quitter;
}
