#include <functional>

#include "TMS_Game.hpp"

TMS_Game::TMS_Game() :
    _isRunning(false),
    _loadingState(LoadingState::GL_ASSETS),
    _currentState(tms::GameState::GAME)
{
}

tms::GameState TMS_Game::loadGameLogic()
{
    if (_isRunning) return tms::GameState::GAME;

    /* Wait for the renderer thread to load OpenGL assets. */
    std::unique_lock<std::mutex> lock(_mutex);
    _waitCondition.wait(lock, std::bind(&TMS_Game::_mainCanLoad, this));

    /* Load main assets. */

    /* Return the mutex and wait for completion. */
    _loadingState = LoadingState::GL_BUFFERS;
    _waitCondition.notify_one();
    _waitCondition.wait(lock, std::bind(&TMS_Game::_mainCanLoad, this));

    return tms::GameState::GAME;
}

void TMS_Game::loadOpenGLAssets()
{
    if (_isRunning) return;

    /* Get the mutex and ensure that the current thread can proceed. */
    std::unique_lock<std::mutex> lock(_mutex);
    _waitCondition.wait(lock, std::bind(&TMS_Game::_rendererCanLoad, this));

    /* Load OpenGL assets. */

    /* Return the mutex and wait for the main thread to load its assets. */
    _loadingState = LoadingState::MAIN_ASSETS;
    _waitCondition.notify_one();
    _waitCondition.wait(lock, std::bind(&TMS_Game::_rendererCanLoad, this));

    /* Load OpenGL buffers. */

    /* Notify the main thread that loading is complete. */
    _loadingState = LoadingState::COMPLETE;
    _waitCondition.notify_one();
}

tms::GameState TMS_Game::gameLoop()
{
    SDL_Event event; // Contains SDL events. 
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

        /* Event polling. */
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
    }
}

void TMS_Game::renderLoop(tms::window_t& window)
{
    while (_currentState == tms::GameState::GAME)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(window.get());
    }
}

bool TMS_Game::_mainCanLoad()
{
    return (_loadingState == LoadingState::MAIN_ASSETS || _loadingState == LoadingState::COMPLETE);
}

bool TMS_Game::_rendererCanLoad()
{
    return (_loadingState != LoadingState::MAIN_ASSETS);
}
