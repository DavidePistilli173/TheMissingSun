#ifndef TMS_HPP
#define TMS_HPP

#include <memory>
#include <SDL.h>

namespace tms
{
    /***************** TYPE ALIASES *****************/
    /* Game window type. */
    using window_t = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

    /***************** CONSTANTS *****************/
    /* String used for the name of the window. */
    constexpr char GAME_NAME[] = "The Missing Sun";
    /* Default window width. */
    constexpr int W_DEF_WIDTH = 800;
    /* Default window height. */
    constexpr int W_DEF_HEIGHT = 600;
    /* Default colour. */
    constexpr float DEFAULT_RED = 0.2f;
    constexpr float DEFAULT_GREEN = 0.2f;
    constexpr float DEFAULT_BLUE = 0.2f;
}

#endif