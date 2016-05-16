#include "demineur.h"

bool demineur(void)
{
    SDL_Surface *screen = NULL;
    SDL_Rect posBlocks[400]; /* pour testPos(); */
    SDL_Event event_demineur;
    IMAGE images;
    FILE* fichier = NULL;

    bool continuer = true, blockVisible[400], rejouer = true, drapeau[400], changement, finJeux = true;
    int i, nombreCases, valeurCases[400], nombreBombes, posSouris, nombreCasesRestante, tailleCases, niveau, nombreDrapeauRestant, temp;
    char difficulte[10] = "MOYEN";
    const int jeu = DEMINEUR;
    niveau = MOYEN;

    images.fond = IMG_Load("Jeux/Demineur/sources/fondDemineur.png");
    verifChargImage(images.fond);
    screen = SDL_GetVideoSurface();
    verifSetVideo(screen);
    SDL_WM_SetCaption("Demineur", NULL);
    fichier = fopen("Jeux/demineur/sources/Difficulte.txt","w+");
    verifChargTexte(fichier);
    fputc(0, fichier);
    fclose(fichier);

    /**************************************************************************/
    while(rejouer)
    {
        initialiseNiveau(niveau, &nombreBombes, &tailleCases, &nombreCases, &nombreCasesRestante, &nombreDrapeauRestant, difficulte , posBlocks);
        initialiseGrilleDemineur(nombreBombes, valeurCases, nombreCases, blockVisible, valeurCases, drapeau);
        initialiseImagesDemineur(&images, tailleCases);
        afficheGrilleDemineur(images, nombreCases, valeurCases, blockVisible, tailleCases, drapeau, niveau);
        afficheScoreDemineur(difficulte, nombreBombes, nombreCasesRestante);
        SDL_Flip(screen);
        continuer = true, changement = false;

        while(continuer)
        {
            SDL_WaitEvent(&event_demineur);
            switch(event_demineur.type)
            {
            case SDL_QUIT:
                continuer = rejouer = finJeux = false;
                break;
            case SDL_KEYDOWN:
                switch (event_demineur.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    if(afficherMenuJeux(jeu))
                        continuer = rejouer = false;
                    else
                    {
                        fichier = fopen("Jeux/demineur/sources/Difficulte.txt","r");
                        verifChargTexte(fichier);
                        temp = (fgetc(fichier)) ? DUR : MOYEN;
                        if(niveau != temp)
                        {
                            changement = true;
                            continuer = false;
                        }
                        fclose(fichier);
                        afficheGrilleDemineur(images, nombreCases, valeurCases, blockVisible, tailleCases, drapeau, niveau);
                        afficheScoreDemineur(difficulte, nombreBombes, nombreCasesRestante);
                        SDL_Flip(screen);
                    }
                    break;
                case SDLK_SPACE:
                    changement = true;
                    continuer = false;
                default :
                    break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch(event_demineur.button.button)
                {
                case SDL_BUTTON_LEFT:
                    posSouris = testPos(posBlocks,tailleCases,tailleCases, nombreCases, event_demineur);
                    if (posSouris > -1)
                    {
                        if(blockVisible[posSouris] != true)
                        {
                            blockVisible[posSouris] = true;
                            if(valeurCases[posSouris] == -1)
                                continuer = false;
                            nombreCasesRestante--;
                            if(drapeau[posSouris] == true)
                            {
                                drapeau[posSouris] = false;
                                nombreDrapeauRestant++;
                            }
                            afficheGrilleDemineur(images, nombreCases, valeurCases, blockVisible, tailleCases, drapeau, niveau);
                            afficheScoreDemineur(difficulte, nombreBombes, nombreCasesRestante);
                            SDL_Flip(screen);
                        }
                    }
                    else if(posSouris == -2)
                    {
                        if(afficherMenuJeux(jeu))
                            continuer = rejouer = false;
                        else
                        {
                            fichier = fopen("Jeux/demineur/sources/Difficulte.txt","r");
                            verifChargTexte(fichier);
                            temp = (fgetc(fichier)) ? DUR : MOYEN;
                            if(niveau != temp)
                            {
                                changement = true;
                                continuer = false;
                            }
                            fclose(fichier);
                            afficheGrilleDemineur(images, nombreCases, valeurCases, blockVisible, tailleCases, drapeau, niveau);
                            afficheScoreDemineur(difficulte, nombreBombes, nombreCasesRestante);
                            SDL_Flip(screen);
                        }
                    }
                    break;

                case SDL_BUTTON_RIGHT:
                    posSouris = testPos(posBlocks,tailleCases,tailleCases, nombreCases, event_demineur);
                    if(posSouris != -1)
                    {
                        if(blockVisible[posSouris] == false && drapeau[posSouris] == false && nombreDrapeauRestant != 0)
                        {
                            drapeau[posSouris] = true;
                            nombreDrapeauRestant--;
                            if(nombreDrapeauRestant == 0)
                            {
                                continuer = verifieDrapeau(drapeau,valeurCases, nombreCases, nombreBombes);
                            }
                            afficheGrilleDemineur(images, nombreCases, valeurCases, blockVisible, tailleCases, drapeau, niveau);
                            afficheScoreDemineur(difficulte, nombreBombes, nombreCasesRestante);
                            SDL_Flip(screen);
                        }
                    }
                    break;

                default:
                    break;
                }
            default:
                break;
            }
        }
        /*****/
        if(finJeux&&rejouer)
        {
        SDL_Delay(500);
        for(i=0; i<nombreCases; i++)
        {
            blockVisible[i] = true;
        }
        afficheGrilleDemineur(images, nombreCases, valeurCases, blockVisible, tailleCases, drapeau, niveau);
        afficheScoreDemineur(difficulte, nombreBombes, nombreCasesRestante);
        SDL_Flip(screen);
        if(changement)
        {
            fichier = fopen("Jeux/demineur/sources/Difficulte.txt","w+");
            if(niveau == MOYEN)
            {
                niveau = DUR;
                fputc(1, fichier);
            }
            else
            {
                niveau = MOYEN;
                fputc(0, fichier);
            }
            fclose(fichier);
        }
        SDL_Delay(2000);
        }
    }
    SDL_FreeSurface(images.fond);
    SDL_FreeSurface(images.vide);
    SDL_FreeSurface(images.mine);
    SDL_FreeSurface(images.carre);
    SDL_FreeSurface(images.drapeau);
    return finJeux;
}
