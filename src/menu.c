#include "menu.h"
/* menu.c contient les fonctions responsables de la gestion du menu dans un jeu. */

#include "include.h"
#include "jeux.h"
#include "fonction_Pendu.h"

bool afficherMenuJeux(int jeu)
{
    SDL_Surface *ecran =NULL,*fond = NULL;
    bool continuer = true;
    int click;
    Sint16 i;
    SDL_Event event_menuJeux;
    SDL_Rect posFond,posBouton[3]; /* [0] => Reprendre ,[1] => Option , [2] => quitter */
    iniPos(&posFond,0,0);
    for(i=0; i<3; i++)
    {
        iniPos(&posBouton[i],102,(Sint16)(200 + 110*i));
    }
    ecran = SDL_GetVideoSurface();
    fond = IMG_Load("Images/menuJeux.png");
    verifChargImage(fond);
    verifSetVideo(ecran);
    fond = IMG_Load("Images/menuJeux.png");
    SDL_BlitSurface(fond,NULL,ecran,&posFond);
    SDL_FreeSurface(fond);
    SDL_Flip(ecran);
    while(continuer)
    {
        SDL_WaitEvent(&event_menuJeux);
        switch(event_menuJeux.type)
        {
        case SDL_QUIT:
            exit(EXIT_SUCCESS);
            break;
        case SDL_KEYDOWN:
            switch(event_menuJeux.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                return false; /* Même comportement que si on clique sur reprendre */
                break;
            default:
                break;
            }
        case SDL_MOUSEBUTTONUP:
            switch(event_menuJeux.button.button)
            {
            case SDL_BUTTON_LEFT:
                click = testPos(posBouton,394,70,3,event_menuJeux);
                switch(click)
                {
                case 0: /* reprendre */
                    continuer = false;
                    break;
                case 1: /* option */
                    if(options(jeu))
                        return true;
                        fond = IMG_Load("Images/menuJeux.png");
                        SDL_BlitSurface(fond,NULL,ecran,&posFond);
                        SDL_FreeSurface(fond);
                        SDL_Flip(ecran);
                    break;
                case 2: /*quitter*/
                    return true;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
    return continuer;
}

bool options(int jeu)
{
    SDL_Surface *ecran =NULL,*fond = NULL, *musiqueA, *musiqueD;
    bool continuer = true, recommencerJeu = false, musique, blit;
    int click, i,j = 0;
    char mot[10] ="",lettreEntree ='\t';
    SDL_Event event_Options;
    SDL_Rect pos, posMusique[1], posMusiqueA, posMusiqueD,retour[1];
    /** initialisation Demineur **/
    FILE* fichier = NULL;
    bool diff = false;
    SDL_Rect posBoutonD[1], posImages[1];
    SDL_Surface *imagesDemineur;
    /** initialisation Pong **/
    SDL_Rect posBoutonPG[3];
    /** initialisation Pendu **/
    SDL_Rect posBoutonP[2];

    /** initialisation Menu **/
    iniPos(&pos,0,0);
    iniPos(&posMusique[0], 89, 193);
    iniPos(&posMusiqueA, 94, 182);
    iniPos(&posMusiqueD, 93, 194);
    iniPos(&retour[0], 100, 500);

    musiqueA = IMG_Load("Images/active.png");
    musiqueD = IMG_Load("Images/desactive.png");



    ecran = SDL_GetVideoSurface();
    /***************** Initialisation de l'écran de fond selon les jeux**********************/
    switch(jeu)
    {
    case DEMINEUR:
        fond = IMG_Load("Images/OptionsDemineur.png");
        SDL_BlitSurface(fond,NULL,ecran,&pos);
        SDL_FreeSurface(fond);
        /** initialisation des variables **/
        iniPos(&posBoutonD[0],91,263);
        iniPos(&posImages[0], 91, 263);
        fichier = fopen("Jeux/demineur/sources/Difficulte.txt","r");
        diff = (fgetc(fichier)) ? true : false; /* 0 = false / 1 = true */
        if(diff)
            imagesDemineur = IMG_Load("Images/difficile.png");
        else
            imagesDemineur = IMG_Load("Images/moyenne.png");
        fclose(fichier);
        verifChargImage(imagesDemineur);
        SDL_BlitSurface(imagesDemineur,NULL,ecran,&posImages[0]);
        SDL_FreeSurface(imagesDemineur);
        break;

    case PONG:
        fond = IMG_Load("Images/OptionsPong.png");
        SDL_BlitSurface(fond,NULL,ecran,&pos);
        SDL_FreeSurface(fond);
        /** initialisation des variables **/
        for(i=0; i<3; i++)
        {
            iniPos(&posBoutonPG[i], 102, (Sint16)(274 + 63*i));
        }
        break;

    case PENDU:
        fond = IMG_Load("Images/OptionsPendu.png");
        SDL_BlitSurface(fond,NULL,ecran,&pos);
        SDL_FreeSurface(fond);
        /** initialisation des variables **/
        for(i=0; i<2; i++)
        {
            iniPos(&posBoutonP[i], (Sint16)(130 + 172*i + i) , 399);
        }
        break;
    default:
        fond = IMG_Load("Images/OptionsBase.png");
        SDL_BlitSurface(fond,NULL,ecran,&pos);
        SDL_FreeSurface(fond);
        break;
    }

    if(!Mix_PausedMusic())
    {
        musique = true;
        SDL_BlitSurface(musiqueA,NULL,ecran,&posMusiqueA); /* musique activé */
    }
    else
    {
        musique = false;
        SDL_BlitSurface(musiqueD,NULL,ecran,&posMusiqueD);
    }

    SDL_Flip(ecran);

    /********************* traitement des options *****************************/
    while(continuer)
    {
        blit = false;
        SDL_WaitEvent(&event_Options);
        switch(event_Options.type)
        {
        case SDL_QUIT:
            SDL_FreeSurface(musiqueA);
            SDL_FreeSurface(musiqueD);
            return true;
            break;

        case SDL_KEYDOWN:
            switch(event_Options.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = false;
                break;

            default:
                break;
            }
        case SDL_MOUSEBUTTONUP:
            switch(event_Options.button.button)
            {
            case SDL_BUTTON_LEFT:
                /*** Case musique ***/
                click = testPos(posMusique, 361, 41, 1,event_Options);
                if(click == 0)
                {
                    if(musique)
                    {
                        Mix_PauseMusic();
                        musique = false;
                    }
                    else
                    {
                        Mix_ResumeMusic();
                        musique = true;
                    }
                }
                /*** Case retour ***/
                click= testPos(retour, 200, 40, 1,event_Options);
                if(click > -1)
                {
                    continuer = false;
                }
                else
                {
                    /*** traitement par jeu ***/
                    switch(jeu)
                    {
                    case DEMINEUR:
                        blit = true;
                        fond = IMG_Load("Images/OptionsDemineur.png");
                        SDL_BlitSurface(fond,NULL,ecran,&pos);
                        SDL_FreeSurface(fond);
                        click = testPos(posBoutonD, 363, 88, 1, event_Options);
                        switch(click)
                        {
                        case 0:
                            diff = !diff;
                            fichier = fopen("Jeux/demineur/sources/Difficulte.txt","w+");
                            (diff) ? fputc(1,fichier) : fputc(0,fichier);
                            fclose(fichier);
                            break;

                        default:
                            break;
                        }
                        imagesDemineur = (diff) ? IMG_Load("Images/difficile.png") : IMG_Load("Images/moyenne.png");
                        SDL_BlitSurface(imagesDemineur,NULL,ecran,&posImages[0]);
                        SDL_FreeSurface(imagesDemineur);
                        break;

                case PONG:
                    blit = true;
                    fond = IMG_Load("Images/OptionsPong.png");
                    SDL_BlitSurface(fond,NULL,ecran,&pos);
                    SDL_FreeSurface(fond);
                    /**/
                    click = testPos(posBoutonPG, 370, 45, 3, event_Options);
                    switch(click)
                    {
                    case 0:
                        ecrireTexte(170);
                        continuer = false;
                        break;
                    case 1:
                        ecrireTexte(100);
                        continuer = false;
                        break;
                    case 2:
                        ecrireTexte(60);
                        continuer = false;
                        break;
                    default:
                        ecrireTexte(100);
                        break;
                    }
                    break;
                    case PENDU:
                        blit = true;
                        fond = IMG_Load("Images/OptionsPendu.png");
                        SDL_BlitSurface(fond,NULL,ecran,&pos);
                        SDL_FreeSurface(fond);
                        while(continuer)
                        {
                            switch(event_Options.type)
                            {
                            break;
                            case SDL_KEYDOWN:
                                switch (event_Options.key.keysym.sym)
                                {
                                case SDLK_BACKSPACE:
                                    if(j>0)
                                        j--;
                                    mot[j]=' ';
                                    break;
                                case SDLK_ESCAPE:
                                    continuer = false;
                                    break;
                                case SDLK_RETURN:
                                    newMot(mot);
                                    continuer = false;
                                    break;
                                default:
                                    lettreEntree = traitementLettre(event_Options.key.keysym.sym);
                                    if(lettreEntree != '\t')
                                    {
                                        if(j<10)
                                        {
                                            mot[j] = lettreEntree;
                                            j++;
                                        }
                                    }
                                    break;
                                }
                                TTF_write(mot);
                            default:
                                break;
                            }
                            click = testPos(posBoutonP,172,42, 2, event_Options);
                            switch(click)
                            {
                            case 0:
                                newMot(mot);
                                continuer = false;
                                break;
                            case 1:
                                continuer = false;
                                break;
                            default:
                                break;
                            }
                            break;

                        default: /* Morpion /snake */
                            blit = true;
                            fond = IMG_Load("Images/OptionsBase.png");
                            SDL_BlitSurface(fond,NULL,ecran,&pos);
                            SDL_FreeSurface(fond);
                            break;
                            }
                        break;
                    }
                }
            default:
                break;
            }
            break;
        default:
            break;
        }
        if(blit)
        {
            if(musique)
                SDL_BlitSurface(musiqueA,NULL,ecran,&posMusiqueA); /* musique activé */
            else
                SDL_BlitSurface(musiqueD,NULL,ecran,&posMusiqueD);
        }
        SDL_Flip(ecran);
    }

    SDL_FreeSurface(musiqueA);
    SDL_FreeSurface(musiqueD);
    return recommencerJeu;
}

void ecrireTexte(int valeurAEcrire)  /* Modification de la taille de la raquette PONG */
{
    FILE *raquette = NULL;
    raquette = fopen("Jeux/Pong/sources/raquette.txt","w+");
    if (raquette == NULL)
    {
        fprintf(stderr,"Erreur, ouverture de raquette.txt");
        exit(EXIT_FAILURE);
    }
    fprintf(raquette,"%d",valeurAEcrire);
    fclose(raquette);
}
void TTF_write (const char mot []) /* Ecriture new mot sur l'écran*/
{
    SDL_Surface *ecran = SDL_GetVideoSurface(),*fond = NULL, *texte = NULL;
    TTF_Font *police = NULL;
    SDL_Color couleurTexte = {0};
    SDL_Rect posFond ,posTexte;
    iniPos(&posTexte,200,300);
    iniPos(&posFond,0,0);
    police = TTF_OpenFont("Jeux/Pendu/sources/orangejuice.ttf", 45);
    fond = IMG_Load("Images/OptionsPendu.png");
    texte = TTF_RenderText_Blended(police,mot,couleurTexte);
    verifSetVideo(ecran);
    verifChargImage(texte);
    verifChargImage(fond);
    verifChargPolice(police);
    SDL_BlitSurface(fond,NULL,ecran,&posFond);
    SDL_BlitSurface(texte,NULL,ecran,&posTexte);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(fond);
    TTF_CloseFont(police);
}

bool Apropos(void)
{
    bool continuer = true, recommencer = true;
    int tailleX = 183, tailleY = 25;
    SDL_Surface *ecran =NULL,*fond = NULL;
    SDL_Event event_Apropos;
    SDL_Rect pos, posRetour[1];
    iniPos(&pos,0,0);
    iniPos(&posRetour[0], 94, 565);
    fond = IMG_Load("Images/Apropos.png");
    ecran = SDL_GetVideoSurface();
    SDL_BlitSurface(fond,NULL,ecran,&pos);
    SDL_FreeSurface(fond);
    SDL_Flip(ecran);

    while(continuer)
    {
        SDL_WaitEvent(&event_Apropos);
        switch(event_Apropos.type)
        {
        case SDL_QUIT:
            return false;
            break;

        case SDL_KEYDOWN:
            switch(event_Apropos.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = false;
                break;

            case SDLK_BACKSPACE:
                continuer = false;
            default:
                break;
            }
        case SDL_MOUSEBUTTONUP:
            switch(event_Apropos.button.button)
            {
            case SDL_BUTTON_LEFT:
                if(testPos(posRetour,tailleX,tailleY,1,event_Apropos) == 0)
                    continuer = false;
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
    return recommencer;
}
