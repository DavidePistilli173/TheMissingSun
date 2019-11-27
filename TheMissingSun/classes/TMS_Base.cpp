#include <SDL.h>
#include <thread>

#include "../include/glad/glad.h"
#include "TMS_Base.hpp"

TMS_Base::TMS_Base() :
    _window(tms::window_t(nullptr, [](SDL_Window*) {})),
    _windowWidth(tms::W_DEF_WIDTH),
    _windowHeight(tms::W_DEF_HEIGHT),
    _glContext(NULL),
    _exit(false),
    _currentState(GameState::MENU)
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
            SDL_CreateWindow(tms::GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, tms::W_DEF_WIDTH, tms::W_DEF_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP),
            [](SDL_Window* win) {SDL_DestroyWindow(win); }
        );
    if (_window == nullptr)
    {
        printf("Could not create game window.\n");
        printf("%s", SDL_GetError());
        return false;
    }
    /* Get current window resolution. */
    SDL_GetWindowSize(_window.get(), &_windowWidth, &_windowHeight);

    /* Rendering related initialisation. */
    /* Create OpenGL context. */
    _glContext = SDL_GL_CreateContext(_window.get());
    /* Load OpenGL function calls. */
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    /* Set VSync on. */
    SDL_GL_SetSwapInterval(1);
    /* Set OpenGL default clear colour to black. */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    /* Set the OpenGL viewport. */
    glViewport(0, 0, _windowWidth, _windowHeight);
    /* Enable OpenGL depth testing. */
    glEnable(GL_DEPTH_TEST);

    /* Release OpenGL context from the current thread. */
    SDL_GL_MakeCurrent(_window.get(), 0);

    return true;
}

void TMS_Base::menuLoop()
{
    bool done = false; // Loop control variable. 
    SDL_Event event; // Contains SDL events. 

    while (!done)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                done = true;
                _exit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_q:
                    done = true;
                    _exit = true;
                    break;
                }
                break;
            }
        }
    }
}

void TMS_Base::outerLoop()
{
    /* Switch between game states. */
    while (!_exit)
    {
        switch (_currentState)
        {
        /* Start the main loop. */
        case GameState::MENU:
            menuLoop();
            break;
        }
    }
}

void TMS_Base::render()
{
    /* Take control of the OpenGL context. */
    if (SDL_GL_MakeCurrent(_window.get(), _glContext) < 0)
    {
        printf("Could not transfer OpenGL context to rendering thread:\n%s", SDL_GetError());
        _exit = true;
        return;
    }

    /* Rendering loop. */
    while (!_exit)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(_window.get());
    }
}

void TMS_Base::run()
{
    /* Start the game logic thread. */
    std::thread renderThread(&TMS_Base::render, this);
    /* Start the rendering loop. */
    outerLoop();

    /* Wait for the rendering thread to end. */
    renderThread.join();
}
