#ifndef TMS_BASE_HPP
#define TMS_BASE_HPP

#include <memory>
#include <SDL.h>

#include "../include/tms.hpp"

enum class GameState
{
    MENU,
    LOADING,
    GAME
};

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
    /* Main menu loop. */
    void menuLoop();
    /* Outer loop to handle high level game states. */
    void outerLoop();
    /* Render engine. It will be executed on a separate thread. */
    void render();

    /**************** VARIABLES *****************/
    /* WINDOW */
    tms::window_t _window; // Main game window.
    int _windowWidth, _windowHeight; // _window resolution.

    /* OpenGL context. */
    SDL_GLContext _glContext;

    /* General variables. */
    bool _exit; // True when exiting the game
    GameState _currentState; // Current game state.
};
#endif
