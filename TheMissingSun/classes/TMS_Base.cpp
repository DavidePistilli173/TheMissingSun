#include <SDL.h>
#include <thread>

#include "../include/glad/glad.h"
#include "TMS_Base.hpp"

TMS_Base::TMS_Base() :
    _window(tms::window_t(nullptr, [](SDL_Window*) {})),
    _windowWidth(tms::W_DEF_WIDTH),
    _windowHeight(tms::W_DEF_HEIGHT),
    _glContext(NULL),
    _gameRunning(false),
    _exit(false),
    _currentState(tms::GameState::MENU)
{
}

TMS_Base::~TMS_Base()
{
    SDL_GL_DeleteContext(_glContext);
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
    glClearColor(tms::DEFAULT_RED, tms::DEFAULT_GREEN, tms::DEFAULT_BLUE, 1.0f);
    /* Set the OpenGL viewport. */
    glViewport(0, 0, _windowWidth, _windowHeight);
    /* Enable OpenGL depth testing. */
    glEnable(GL_DEPTH_TEST);
    /* Enable alpha blending. */
    glEnable(GL_BLEND);
    /* Set blending factors. */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Initialise the main menu. */
    if (!_menu.init(_windowWidth, _windowHeight))
    {
        printf("Could not load main menu.\n");
        return false;
    }

    return true;
}

void TMS_Base::outerLoop()
{
    /* Switch between game states. */
    while (!_exit)
    {
        switch (_currentState)
        {
        /* Start the menu loop. */
        case tms::GameState::MENU:
            _currentState = _menu.menuLoop(_window, _windowWidth, _windowHeight);
            break;
        /* Load the game. */
        case tms::GameState::LOADING:
            if (_gameRunning) _currentState = tms::GameState::GAME;
            else
            {
                _currentState = _game.loadGame();
                if (_currentState == tms::GameState::EXIT)
                {
                    printf("Error while loading game.\nExiting...\n");
                    _exit = true;
                }
                _gameRunning = true;
            }
            break;
        case tms::GameState::GAME:
            _currentState =_game.gameLoop(_window);
            break;
        /* Exit the game. */
        case tms::GameState::EXIT:
            _exit = true;
            break;
        }
    }
}

void TMS_Base::run()
{
    /* Start the game logic. */
    outerLoop();
}
