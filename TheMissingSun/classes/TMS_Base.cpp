#include <SDL.h>

#include "../include/glad/glad.h"
#include "TMS_Base.hpp"

TMS_Base::TMS_Base() :
    _window(tms::window_t(nullptr, [](SDL_Window*) {})),
    _glContext(NULL)
{
}

bool TMS_Base::init()
{
    /* Set OpenGL version. */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    /* Set core profile for OpenGL. */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    /* Create main window. */
    _window = tms::window_t
        (
            SDL_CreateWindow(tms::GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, tms::W_DEF_WIDTH, tms::W_DEF_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN),
            [](SDL_Window* win) {SDL_DestroyWindow(win); }
        );
    if (_window == nullptr)
    {
        printf("Could not create game window.\n");
        printf("%s", SDL_GetError());
        return false;
    }
    /* Create OpenGL context. */
    _glContext = SDL_GL_CreateContext(_window.get());
    /* Load OpenGL function calls. */
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    /* Set VSync on. */
    SDL_GL_SetSwapInterval(1);


    return true;
}

void TMS_Base::render()
{
}
