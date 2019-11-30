#include "TMS_Menu.hpp"

#include <SDL.h>

TMS_Menu::TMS_Menu()
{
}

tms::GameState TMS_Menu::menuLoop()
{
    SDL_Event event; // Contains SDL events. 

    while (true)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return tms::GameState::EXIT;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_q:
                    return tms::GameState::EXIT;
                    break;
                }
                break;
            }
        }
    }
}
