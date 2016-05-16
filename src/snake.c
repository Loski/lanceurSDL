#include "snake.h"
#include "jeux.h"

bool snake (void)
{
    SDL_Surface *screen = NULL;
    SDL_Rect posBlock;
    SDL_Event event_snake;
    IMAGE images;
    POS tete,fin;

    int valeurCases[22][17]; /* 1 pomme 2 tete 3 droite 4 haut 5 gauche 6 bas */
    Uint32 delay = 400, Ticks, endTicks;
    int direction = DROITE,Case, directionActuelle, score = 0;
    bool continuer = true, rejouer = true, menu, clic = false,quitter = true;
    const int jeu = SNAKE;

    SDL_WM_SetCaption("SNAKE", NULL);
    screen = SDL_GetVideoSurface();
    initialiseImagesSnake(&images);
    verifChargImage(images.fond);
    verifChargImage(images.tete);
    verifChargImage(images.pomme);
    verifChargImage(images.corps);
    verifSetVideo(screen);

iniPos(&posBlock,25,125);
    while(rejouer)
    {
        score = 0;
        delay = 200;
        direction= DROITE;
        afficheScore(score);
        initialiseGrille(valeurCases,&tete,&fin);
        placePomme(valeurCases);
        afficheGrille(images,valeurCases,posBlock);
        SDL_Flip(screen);
        SDL_Delay(delay);
        continuer = true;

        while(continuer)
        {
            directionActuelle = direction;
            endTicks = SDL_GetTicks();
            Ticks = endTicks + delay;
            clic = false;
            while(endTicks < Ticks)
            {
                SDL_PollEvent(&event_snake);
                switch(event_snake.type)
                {
                case SDL_QUIT:
                    continuer = false;
                    rejouer = false;
                    quitter = false;
                    break;

                case SDL_KEYDOWN:
                    switch (event_snake.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        if(clic == false)
                        {
                            if(afficherMenuJeux(jeu))
                            {
                                rejouer = continuer = false;
                            }
                            else
                            {
                                clic = true;
                                event_snake.type = SDLK_1;
                            }
                        }
                        break;

                    case SDLK_LEFT:
                        if(directionActuelle != DROITE)
                            direction = GAUCHE;
                        break;
                    case SDLK_DOWN:
                        if(directionActuelle != HAUT)
                            direction = BAS;
                        break;
                    case SDLK_RIGHT:
                        if(directionActuelle != GAUCHE)
                            direction = DROITE;
                        break;
                    case SDLK_UP:
                        if(directionActuelle != BAS)
                            direction = HAUT;
                        break;

                    default :
                        break;
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    switch(event_snake.button.button)
                    {
                    case SDL_BUTTON_LEFT:
                        if(clic == false)
                        {
                            menu = testMenu(event_snake);
                            if(menu == true)
                            {

                                if(afficherMenuJeux(jeu))
                                {
                                    rejouer = continuer = false;
                                }
                                else
                                {
                                    event_snake.type = SDLK_1;
                                    clic = true;
                                }
                            }
                        }
                        break;

                    default:
                        break;
                    }
                    break;

                default:
                    break;
                }
                SDL_Delay(10);
                endTicks = SDL_GetTicks();
            }
            Case = deplaceSnake(direction,&tete,&fin,valeurCases);
            if(Case < 0)
                continuer = false;
            if(Case == 1)
            {
                placePomme(valeurCases);
                if(delay >= 100)
                    delay -= 10;
                score++;
            }

            afficheGrille(images,valeurCases,posBlock);
            afficheScore(score);
            SDL_Flip(screen);
        }

        SDL_Delay(2000);

    SDL_FreeSurface(images.fond);
    SDL_FreeSurface(images.tete);
    SDL_FreeSurface(images.corps);
    SDL_FreeSurface(images.pomme);
    }

    return quitter;
}
