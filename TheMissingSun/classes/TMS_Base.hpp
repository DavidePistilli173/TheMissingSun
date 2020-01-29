#ifndef TMS_BASE_HPP
#define TMS_BASE_HPP

#include <SDL.h>

#include "../include/tms.hpp"
#include "TMS_Game.hpp"
#include "TMS_Menu.hpp"

/* Base class. */
class TMS_Base
{
public:
    /* Default constructor. */
    TMS_Base();

    /**************** METHODS ****************/
    /* Initialise the basic game data. 
       Return false on error. */
    bool init();
    /* Start and run the game logic. */
    void run();

private:
    /**************** METHODS ****************/
    /* Outer loop to handle high level game states. */
    void outerLoop();
    /* Render engine. It will be executed on a separate thread. */
    void render();

    /**************** VARIABLES *****************/
    /* Window. */
    tms::window_t _window; // Main game window.
    int _windowWidth, _windowHeight; // _window resolution.

    /* Graphics. */
    SDL_GLContext _glContext;

    /* General variables. */
    bool _gameRunning;
    bool _exit; // True when exiting the game
    tms::GameState _currentState; // Current game state.

    /* Game. */
    TMS_Menu _menu; // Game menu logic and data.
    TMS_Game _game; // Main game logic and data.
};
#endif
