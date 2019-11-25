#ifndef TMS_BASE_HPP
#define TMS_BASE_HPP

#include <memory>
#include <SDL.h>

#include "../include/tms.hpp"

class TMS_Base
{
public:
    /* Default constructor. */
    TMS_Base();

    /* Initialise the basic game data. 
       Return false on error. */
    bool init();

private:
    /**************** METHODS ****************/
    /* Render engine. It will be executed on a separate thread. */
    void render();

    /**************** VARIABLES *****************/
    /* WINDOW */
    tms::window_t _window; // Main game window.
    int _windowWidth, _windowHeight; // _window resolution.

    /* OpenGL context. */
    SDL_GLContext _glContext;
};
#endif
