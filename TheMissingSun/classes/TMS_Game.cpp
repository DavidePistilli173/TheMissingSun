#include "TMS_Game.hpp"

TMS_Game::TMS_Game() :
    _isRunning(false),
    _currentState(tms::GameState::GAME)
{
}



tms::GameState TMS_Game::loadGame()
{
    return tms::GameState::GAME;
}

tms::GameState TMS_Game::gameLoop(tms::window_t& window)
{
    _clock.startClock();
    while (true)
    {
        /* If the last frame updated too quickly, wait for the remaining time. */
        int updateTime = _clock.getTime();
        if (updateTime < tms::MENU_UPDATE_TIME)
        {
            SDL_Delay(tms::MENU_UPDATE_TIME - updateTime);
            updateTime = _clock.getTime(); // Update the elapsed time.
        }
        _clock.startClock();

        /* Event handling. */
        tms::GameState newState = handleEvents();
        if (newState != tms::GameState::GAME) return newState;

        /* Rendering. */
        render(window);
    }
}

tms::GameState TMS_Game::handleEvents()
{
    /* Event polling. */
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            _currentState = tms::GameState::EXIT;
            return tms::GameState::EXIT;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                _currentState = tms::GameState::MENU;
                return tms::GameState::MENU;
                break;
            }
            break;
        }
    }
    return tms::GameState::GAME;
}

void TMS_Game::render(tms::window_t& window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SDL_GL_SwapWindow(window.get());
}
