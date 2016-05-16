#include "include.h"
#include "jeux.h"
int main(int argc,char** argv)
{
    SDL_Surface *fond = NULL,*screen = NULL,*icone = NULL;
    Mix_Music *musique = NULL;
    SDL_Rect posCases[3],posFond;
    SDL_Event event;

    int choix, i,TX = 560, TY = 62, n = 3;
    bool continuer = true;
    iniPos(&posFond,0,0);
    for(i=0; i<3; i++)
    {
        posCases[i].x = 40;
        posCases[i].y = (Sint16)(190 + (100 * i));
    }
    init();
    screen = SDL_SetVideoMode(LARGEUR_FENETRE,HAUTEUR_FENETRE,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
    verifSetVideo(screen);
    fond = IMG_Load("Images/Menu.png");
    icone = IMG_Load("Images/ICO.png");
    verifChargImage(icone);
    verifChargImage(fond);
    SDL_WM_SetIcon(icone,NULL);
    srand((unsigned int)time(NULL));
    musique = Mix_LoadMUS("Son/TalkToMe.mp3");
    Mix_PlayMusic(musique, -1);

    while(continuer)
    {
        SDL_WM_SetCaption("Menu", NULL);
        SDL_BlitSurface(fond, NULL,screen, &posFond);
        SDL_Flip(screen);
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer=false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer=false;
                break;
            default:
                break;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            switch(event.button.button)
            {
            case SDL_BUTTON_LEFT:
                choix = testPos(posCases, TX, TY, n,event);
                switch(choix)
                {
                case 0:
                    continuer = JEUX();
                    break;

                case 1:
                    continuer = Apropos();
                    break;

                case 2:
                    continuer = false;
                    break;
                default:
                    break;
                }
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }
    }
    Mix_FreeMusic(musique);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(icone);
    SDL_FreeSurface(fond);
    closeSDL();
    return EXIT_SUCCESS;
}



