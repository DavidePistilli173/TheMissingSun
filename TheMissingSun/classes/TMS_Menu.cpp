#include "TMS_Menu.hpp"

#include <SDL.h>

TMS_Menu::TMS_Menu()
{
}

tms::GameState TMS_Menu::menuLoop()
{
    SDL_Event event; // Contains SDL events. 
    _clock.startClock();

    while (true)
    {
        /* If the last frame updated too quickly, wait for the remaining time. */
        int updateTime = _clock.getTime();
        if (updateTime < tms::MENU_UPDATE_TIME) SDL_Delay(tms::MENU_UPDATE_TIME - updateTime);
        _clock.startClock();

        /* Event polling. */
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
