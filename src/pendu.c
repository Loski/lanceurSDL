#include "pendu.h"

bool pendu(bool *rejouer)
{
    /**************************************VARIABLE*************************************************/
    SDL_Event event_pendu;
    const SDL_Color couleurTexte = {0,0,0,0};
    SDL_Surface *iPendu = NULL,*ecran =NULL, *texte[3] = {NULL},*imageDeFond = NULL; /* txt[0] = motBuffer , txt[1] = Resultat  ok et txt[2] = Lettrefausse ok*/
    SDL_Rect positionPendu,positionImageDeFond,positionTexte[3];
    TTF_Font *police = NULL;
    char imagePendu[50] = "";
    size_t taille;
    unsigned int nombreDeCoupRestant = 1,i=0,j=0,compteur = 0;
    char lettreEntree = '\t';
    bool continuer = true,menu = true;
    char *motBuffer = NULL,*motMystere = NULL,*lettreTest =NULL;
    char lettreFausse[8] = "";
    const int jeu = PENDU;

    SDL_WM_SetCaption("Le Pendu", NULL);
    ecran = SDL_GetVideoSurface();
    imageDeFond = IMG_Load("Jeux/Pendu/sources/image_pendu.png");
    police = TTF_OpenFont("Jeux/Pendu/sources/orangejuice.ttf", 65);
    verifChargImage(imageDeFond);
    verifChargPolice(police);
    verifSetVideo(ecran);

    /*************initialisation des positions ******/
    iniPos(&positionImageDeFond,0,0);
    iniPos(&positionPendu,LARGEUR_PENDU,HAUTEUR_PENDU);
    iniPos(&positionTexte[0],LARGEUR_TEXTE,HAUTEUR_TEXTE);
    iniPos(&positionTexte[1],LARGEUR_TEXTE2,HAUTEUR_TEXTE2);
    iniPos(&positionTexte[2],LARGEUR_LETTRE,HAUTEUR_LETTRE);
    motMystere = piocherMot(); /* on cherche un mot au hasard */

    taille = strlen(motMystere);
    motBuffer = calloc((taille+1),sizeof(char));
    if(motMystere == NULL || motBuffer == NULL)
    {
        exit(6);
    }
    for(i=0; i<taille; i++)
    {
        motBuffer[i]= '-'; /* On cache le mot */
    }
    texte[0] = TTF_RenderText_Blended(police,motBuffer,couleurTexte);
    sprintf(imagePendu,"Jeux/Pendu/sources/%d.png",nombreDeCoupRestant-1);
    iPendu = IMG_Load(imagePendu);
    verifChargImage(iPendu);
    verifChargImage(texte[0]);
    SDL_BlitSurface(imageDeFond,NULL,ecran,&positionImageDeFond);
    SDL_BlitSurface(iPendu,NULL,ecran,&positionPendu);
    SDL_BlitSurface(texte[0],NULL,ecran,&positionTexte[0]);

    while(continuer)
    {
        SDL_Flip(ecran);
        SDL_WaitEvent(&event_pendu);
        switch(event_pendu.type)
        {
        case SDL_QUIT:  /*On quitte le launcher */
            continuer = false;
            *rejouer = false;
            menu = false;
            break;
        case SDL_MOUSEBUTTONUP:
            switch(event_pendu.button.button)
            {
            case SDL_BUTTON_LEFT: /* On regarde si on a cliqué sur le bouton menu */
                if(testMenu(event_pendu))
                {
                    if(afficherMenuJeux(jeu)) /*On quitte le jeu  */
                    {
                        continuer = false;
                        *rejouer = false;
                    }
                    else
                    {
                        /*On reblitte le jeu */
                        SDL_BlitSurface(imageDeFond,NULL,ecran,&positionImageDeFond);
                        SDL_BlitSurface(iPendu,NULL,ecran,&positionPendu);
                        SDL_BlitSurface(texte[0],NULL,ecran,&positionTexte[0]);
                        SDL_BlitSurface(texte[2],NULL,ecran,&positionTexte[2]);
                    }
                }
                break;
            default:
                break;
            }
        case SDL_KEYDOWN:
        {
            switch(event_pendu.key.keysym.sym)
            {
            case SDLK_ESCAPE: /* Menu*/
                if(afficherMenuJeux(jeu))
                {
                    continuer = false;
                    *rejouer = false;
                }
                else
                {
                    SDL_BlitSurface(imageDeFond,NULL,ecran,&positionImageDeFond);
                    SDL_BlitSurface(iPendu,NULL,ecran,&positionPendu);
                    SDL_BlitSurface(texte[0],NULL,ecran,&positionTexte[0]);
                    SDL_BlitSurface(texte[2],NULL,ecran,&positionTexte[2]);
                }
                break;
            default :
                lettreEntree=event_pendu.key.keysym.sym;
                break;
            }
            lettreEntree = traitementLettre(lettreEntree);
            if(lettreEntree != '\t')
            {
                compteur = 0;
                for(i=0; i<taille; i++)
                {
                    if(motBuffer[i]==motMystere[i] && lettreEntree != motMystere[i])
                        /* On regarde si la lettre à un rang i, est identique dans motBuffer et motMystère,
                        lettreEntree != motMystere[i] sert à ce qu'une lettre précédemment entrée, et comprise dans le motMystère donc dans mot buffer ne modifie pas le compteur de coup restant*/
                    {
                        compteur++;
                    }
                    else if(lettreEntree==motMystere[i])
                    {
                        motBuffer[i] = lettreEntree;
                    }
                    else
                    {
                        compteur++;
                    }
                }
                if(compteur == taille) /* donc si la lettre n'est pas présente dans le mot */
                {
                    if(j<7)
                    {
                        lettreTest = NULL;
                        lettreTest = strchr(lettreFausse,lettreEntree);
                        /*strchr renvoie NULL si caractère non trouvé*/
                        if(lettreTest == NULL)
                        {
                            nombreDeCoupRestant++;
                            lettreFausse[j]=lettreEntree;
                            j++;
                            SDL_FreeSurface(iPendu);
                            SDL_FreeSurface(texte[2]);
                            sprintf(imagePendu,"Jeux/Pendu/sources/%d.png",nombreDeCoupRestant-1);
                            texte[2] = TTF_RenderText_Blended(police,lettreFausse,couleurTexte);
                            iPendu = IMG_Load(imagePendu);
                            verifChargImage(texte[2]);
                            verifChargImage(iPendu);
                        }

                    }

                }
                else
                {
                    SDL_FreeSurface(texte[0]);
                    texte[0] = TTF_RenderText_Blended(police,motBuffer,couleurTexte);
                }
                SDL_BlitSurface(imageDeFond,NULL,ecran,&positionImageDeFond);
                SDL_BlitSurface(texte[0],NULL,ecran,&positionTexte[0]);
                SDL_BlitSurface(iPendu,NULL,ecran,&positionPendu);
                SDL_BlitSurface(texte[2],NULL,ecran,&positionTexte[2]);
            }
            break;
        }
        default:
            break;
        }
        if(nombreDeCoupRestant ==  8)
        {
            SDL_FreeSurface(iPendu);
            sprintf(imagePendu,"Jeux/Pendu/sources/%d.png",nombreDeCoupRestant-1);
            iPendu = IMG_Load(imagePendu);
            verifChargImage(iPendu);
            texte[1] = TTF_RenderText_Blended(police,"Vous avez perdu!",couleurTexte);
            SDL_BlitSurface(iPendu,NULL,ecran,&positionPendu);
            SDL_BlitSurface(texte[1],NULL,ecran,&positionTexte[1]);
            continuer = false;
        }
        if(!strcmp(motBuffer,motMystere))  /* Si motBuffer et motMystere sont identiques alors victoire */
        {
            SDL_FreeSurface(texte[0]);
            texte[0] = TTF_RenderText_Blended(police,motMystere,couleurTexte);
            texte[1] = TTF_RenderText_Blended(police,"Vous avez Gagne!",couleurTexte);
            SDL_BlitSurface(imageDeFond,NULL,ecran,&positionImageDeFond);
            SDL_BlitSurface(texte[1],NULL,ecran,&positionTexte[1]);
            SDL_BlitSurface(texte[0],NULL,ecran,&positionTexte[0]);
            continuer = false;
        }
    }
    SDL_Flip(ecran);
    if(*rejouer)
    {
        SDL_Delay(1000);
        while (SDL_PollEvent (&event_pendu));
    }
    free(motBuffer);
    free(motMystere);
    free(lettreTest);
    SDL_FreeSurface(iPendu);
    SDL_FreeSurface(texte[0]);
    SDL_FreeSurface(texte[1]);
    SDL_FreeSurface(texte[2]);
    SDL_FreeSurface(imageDeFond);
    SDL_FreeSurface(ecran);
    return menu;
}
