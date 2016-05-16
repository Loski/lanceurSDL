#include "include.h"
#include "fonctionMorpion.h"


/**********************************************************************************/

bool Morpion()
{
    SDL_Surface *screen = NULL, *texte[3] = {NULL,NULL,NULL};
    SDL_Rect posBlock[9], posFond, posTexte[3];
    SDL_Event event_morpion;
    IMAGE images;

    bool continuer = true, correct = false, rejouer = true, full_case = false,finJeux = true;
    int joueur = 1;
    int blocks[9], i, block_boutton;
    int pointJ[2] = {0}, manche = 1;
    int n = 9;
    const int jeu = MORPION;

    /******************************************************************************/
    screen = SDL_GetVideoSurface();
    verifSetVideo(screen);
    iniPos(&posFond,0,0);

    for(i=0; i<9; i++)
    {
        blocks[i] = RIEN;
        posBlock[i].x = (Sint16)(MIN + ((i%3)*TAILLE));
        posBlock[i].y = (Sint16)(MAX + ((i/3)*TAILLE));
    }

    /* position police */
    for(i=0; i<2; i++)
    {
        posTexte[i].x = 125;
        posTexte[i].y = (Sint16)(555 + (22*i));
    }

    iniPos(&posTexte[2],322,577);

    /******************************************************************************/

    initialiseImagesMorpion(&images);
    verifChargImage(images.fond);
    verifChargImage(images.croix);
    verifChargImage(images.rond);

    /******************************************************************************/

    SDL_WM_SetCaption("Morpion", NULL);

    /******************************************************************************/

    SDL_BlitSurface(images.fond, NULL,screen, &posFond);
    SDL_Flip(screen);


    while(rejouer)
    {
        full_case = false;
        continuer = true;
        while(continuer == true)
        {
            block_boutton = 0;
            if (joueur == 1)
                joueur++;
            else
                joueur--;
            /**********/
            if (joueur == 2) /*IA*/
            {
                i = verifie(blocks);
                if (i != -1)
                {
                    blocks[i] = ROND;
                    if(check(blocks,&full_case))
                        continuer=false;
                }
                else if(continuer)
                {
                    do
                    {
                        i=(rand()%9);
                    }
                    while(blocks[i] != RIEN);
                    blocks[i] = ROND;
                }

                if(check(blocks,&full_case))
                    continuer=false;
            }
            /**********/
            else if (joueur&&continuer)
            {
                do
                {
                    correct = false;
                    SDL_WaitEvent(&event_morpion);
                    switch(event_morpion.type)
                    {
                    case SDL_QUIT:
                        correct = true;
                        continuer = false;
                        rejouer = false;
                        finJeux = false;
                        break;

                    case SDL_KEYDOWN:
                        switch (event_morpion.key.keysym.sym)
                        {
                        case SDLK_ESCAPE:
                            if(afficherMenuJeux(jeu))
                            {
                                correct = true;
                                continuer = false;
                                rejouer = false;
                            }
                            else
                            {
                                initialiseImagesMorpion(&images);
                                SDL_BlitSurface(images.fond,NULL,screen,&posFond);
                                affiche_score(texte, manche,pointJ, posTexte);

                                for(i=0; i<9; i++)
                                {
                                    if(blocks[i]==CROIX)
                                        SDL_BlitSurface(images.croix, NULL,screen, &posBlock[i]);
                                    else if(blocks[i]==ROND)
                                        SDL_BlitSurface(images.rond, NULL,screen, &posBlock[i]);

                                }
                                SDL_Flip(screen);
                            }
                            break;

                        default :
                            break;
                        }
                        break;

                    case SDL_MOUSEBUTTONUP:
                        switch(event_morpion.button.button)
                        {
                        case SDL_BUTTON_LEFT:
                            block_boutton = testPos(posBlock, TAILLE,TAILLE,n,event_morpion);
                            if (block_boutton > -1 && blocks[block_boutton] == RIEN)
                            {
                                blocks[block_boutton] = CROIX;
                                if(check(blocks, &full_case))
                                {
                                    continuer=false;
                                }
                                correct = true;
                            }
                            else if(block_boutton == -2)
                            {
                                if(afficherMenuJeux(jeu))
                                {
                                    correct = true;
                                    continuer = false;
                                    rejouer = false;
                                }
                                else
                                {
                                    initialiseImagesMorpion(&images);
                                    SDL_BlitSurface(images.fond,NULL,screen,&posFond);
                                    affiche_score(texte, manche,pointJ, posTexte);

                                    for(i=0; i<9; i++)
                                    {
                                        if(blocks[i]==CROIX)
                                            SDL_BlitSurface(images.croix, NULL,screen, &posBlock[i]);
                                        else if(blocks[i]==ROND)
                                            SDL_BlitSurface(images.rond, NULL,screen, &posBlock[i]);

                                    }
                                    SDL_Flip(screen);
                                }
                            }
                            break;

                        default :
                            break;
                        }
                        break;

                    default :
                        break;
                    }
                }
                while(!correct);
            }
            SDL_BlitSurface(images.fond, NULL,screen, &posFond);
            for(i=0; i<9; i++)
            {
                if(blocks[i]==CROIX)
                    SDL_BlitSurface(images.croix, NULL,screen, &posBlock[i]);
                else if(blocks[i]==ROND)
                    SDL_BlitSurface(images.rond, NULL,screen, &posBlock[i]);
            }
            affiche_score(texte, manche,pointJ, posTexte);
            SDL_Flip(screen);
        }
        if(rejouer)
        {
            SDL_Delay(1500);
            if(!full_case)
                pointJ[joueur - 1]++;
            manche++;
        }

        for(i=0; i<9; i++)
        {
            blocks[i] = RIEN;
        }

        SDL_BlitSurface(images.fond, NULL,screen, &posFond);
        affiche_score(texte, manche,pointJ, posTexte);
        SDL_Flip(screen);

    }

    /******************************************************************************/

    SDL_FreeSurface(images.fond);
    SDL_FreeSurface(images.croix);
    SDL_FreeSurface(images.rond);
    return finJeux;
}

/**********************************************************************************/
