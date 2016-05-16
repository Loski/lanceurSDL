#include "include.h"
#include "jeux.h"

/******************************Fonction qui récupère les erreurs *********************************************/
void verifSetVideo(SDL_Surface *ecran)
{
    if(ecran == NULL)
    {
        fprintf(stderr, "L'initialisation du mode video n'a pas fonctionne : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}
void verifChargImage(SDL_Surface *image)
{
    if(image == NULL)
    {
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}
void verifChargPolice(TTF_Font *police)
{
    if(police == NULL)
    {
        fprintf(stderr, "L'initialisation de la police n'a pas fonctionne : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}
void verifChargTexte(FILE * texte)
{
    if(texte == NULL)
    {
        fprintf(stderr,"L'initialisation du texte a échoué");
        exit (EXIT_FAILURE);
    }
}
/******************************Fonction qui gère le click gauche de la sourie ****************/
int testPos(SDL_Rect pos[], int tailleX, int tailleY, int n, SDL_Event event)
{
    int i;
    SDL_Rect posMenu;
    iniPos(&posMenu,433,468);
    for(i=0; i<n; i++)
    {
        if(event.button.x > pos[i].x && event.button.x < (pos[i].x +tailleX)) /* On verifie si les positions en abscisses sont concordantes */
        {
            if(event.button.y > pos[i].y && event.button.y < (pos[i].y +tailleY))  /* On verifie si les positions en ordonnées sont concordantes */
            {
                return i;
            }
        }
    }
    if(testMenu(event))
        return -2; /*L'utilisateur à selectionner le menu sur le morpion,le snake ou le démineur
                         Option pong => ECHAP
                         Option pendu => Fonction TestMenu directement puisqu'on gère uniquement le click sur le Menu */


    return -1; /* L'utilisateur n'a pas appuyer sur un bouton */
}

bool testMenu(SDL_Event event)
{
    SDL_Rect posMenu;
    iniPos(&posMenu,433,568);
    if(event.button.x > posMenu.x && event.button.x < 600)
    {
        if(event.button.y > posMenu.y && event.button.y < 600)
        {
            return true;
        }
    }
    return false;
}
/***********Initialisation des structures SDL_Rect ************************/
void iniPos(SDL_Rect *pos,Sint16 largeur,Sint16 hauteur)
{
    pos->x = largeur;
    pos->y = hauteur;
}


bool JEUX(void)
{
    bool continuer = true,quitter = true;
    SDL_Rect posCases[4], posFond,posFleche[2];
    SDL_Surface *fond = NULL, *jeux[5] = {NULL},*ecran = NULL;
    SDL_Event event_jeux;
    int TX = 461, TY = 55, i, choix, n=4,defilement,menu=0;
    iniPos(&posFond,0,0);
    ecran = SDL_GetVideoSurface();
    verifSetVideo(ecran);

    /*******************Chargement des images *****************************/
    fond = IMG_Load("Images/selectionJeux.png");
    SDL_WM_SetCaption("Selection des Jeux", NULL);
    jeux[0] = IMG_Load("Images/selectionMorpion.png");
    jeux[1] = IMG_Load("Images/selectionPendu.png");
    jeux[2] = IMG_Load("Images/selectionSnake.png");
    jeux[3] = IMG_Load("Images/selectionPong.png");
    jeux[4] = IMG_Load("Images/selectionDemineur.png");
    /***********************************************************************/

    verifChargImage(fond);
    for(i = 0; i<5; i++)
    {
        verifChargImage(jeux[i]);
        if(i<4)
        {
            iniPos(&posCases[i],80,((Sint16)(190 + (90 * i))));
            if(i<2)
            {
                iniPos(&posFleche[i],292,(Sint16)(152 + (371*i)));
            }
        }
    }
    SDL_BlitSurface(fond,NULL,ecran,&posFond);
    afficher(0,jeux);

    while(continuer)
    {
        SDL_Flip(ecran);
        SDL_WaitEvent(&event_jeux);
        switch(event_jeux.type)
        {
        case SDL_QUIT:
            continuer = false;
            quitter = false;
            break;
        case SDL_KEYDOWN:
            switch (event_jeux.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = false;
                break;
            case SDLK_UP: /* défilement HAUT*/
                SDL_BlitSurface(fond,NULL,ecran,&posFond);
                afficher(0,jeux);
                menu = 0;
                break;
            case SDLK_DOWN: /*defilement BAS */
                SDL_BlitSurface(fond,NULL,ecran,&posFond);
                afficher(1,jeux);
                menu = 1;
                break;
            default:
                break;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            switch(event_jeux.button.button)
            {
            case SDL_BUTTON_LEFT:
                choix = testPos(posCases, TX, TY, n,event_jeux);/* On test d'abord si l'utilisateur veut lancer un jeu */
                if(choix != -1)
                {
                    continuer = play(choix,menu);
                    if(continuer) /* Si on veut quitter le jeu mais pas le launcher */
                    {
                        SDL_BlitSurface(fond,NULL,ecran,&posFond);
                        afficher(0,jeux);
                        menu = 0;
                    }
                    else
                        quitter = false;
                }
                defilement = testPos(posFleche,17,33,2,event_jeux);/* Puis si l'utilisateur veut défiler l'écran de selection de jeux avec les flêches directionnelles */
                switch(defilement)
                {
                case 0:
                    SDL_BlitSurface(fond,NULL,ecran,&posFond);
                    afficher(0,jeux);
                    menu = 0; /*Placement normal */
                    break;
                case 1:
                    SDL_BlitSurface(fond,NULL,ecran,&posFond);
                    afficher(1,jeux);
                    menu = 1; /* Placement modifié vers le bas */
                    break;
                default:
                    break;
                }
                break;
            case SDL_BUTTON_WHEELUP:/* Defilement HAUT avec la molette */
                SDL_BlitSurface(fond,NULL,ecran,&posFond);
                afficher(0,jeux);
                menu = 0;
                break;
            case SDL_BUTTON_WHEELDOWN: /* Defilement BAS avec la molette */
                SDL_BlitSurface(fond,NULL,ecran,&posFond);
                afficher(1,jeux);
                menu = 1;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
    for(i=0; i<4; i++)
    {
        SDL_FreeSurface(jeux[i]);
    }

    SDL_FreeSurface(fond);
    return quitter;
}

void afficher(int nombre,SDL_Surface *jeux[])
{
    int i,j=0;
    SDL_Surface *ecran =NULL,*fleche[2] = {NULL};
    SDL_Rect posJeux[5],posFleche[2];
    fleche[0] = IMG_Load("Images/flecheHaut.png");
    fleche[1] = IMG_Load("Images/flecheBas.png");
    ecran = SDL_GetVideoSurface();
    verifSetVideo(ecran);
    for(i = 0; i<4; i++)
    {
        iniPos(&posJeux[i],80,(Sint16)(190 + (90 * i)));
        if(i<2)
        {
            verifChargImage(fleche[i]);
            iniPos(&posFleche[i],292,(Sint16)(152 + (371*i)));
            SDL_BlitSurface(fleche[i],NULL,ecran,&posFleche[i]);
            SDL_FreeSurface(fleche[i]);
        }
    }

    for(i=nombre; i<(nombre+4); i++)
    {
        SDL_BlitSurface(jeux[i],NULL,ecran,&posJeux[j]);
        j++; /* La position des images ne change pas selon le défilement */
    }
}

void init(void) /*Initialisation des API SDL */
{
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1 || TTF_Init() == -1) /*Initialisation de l'API SDL et SDL TTF*/
    {
        fprintf(stderr,"Erreur d'initialisation de la SDL : %s\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) /*Initialisation de l'API Mixer*/
    {
        fprintf(stderr,"Erreur d'initialisation du mode audio%s", Mix_GetError());
        exit(EXIT_FAILURE);
    }

}

void closeSDL(void) /* Quitte tous les API SDL */
{
    SDL_Quit();
    TTF_Quit();
    SDL_CloseAudio();
}

bool play(int choix,int menu) /* Lancement des jeux en gérant le déplacement */
{
    bool quitter = true,rejouer = true;
    choix += menu;
    switch(choix)
    {
    case 0:
        quitter = Morpion();
        break;
    case 1:
        do
        {
            quitter = pendu(&rejouer);
        }
        while(rejouer); /* Boucle pour recommencer le Pendu tant qu'on ne veut pas quitter le jeu. */
        break;
    case 2:
        quitter = snake();
        break;
    case 3:
        quitter = pong();
        break;
    case 4:
        quitter = demineur();
        break;
    default:
        break;
    }
    return quitter;
}
