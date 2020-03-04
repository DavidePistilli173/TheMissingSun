#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "TMS_Background.hpp"
#include "TMS_Game.hpp"


TMS_Game::TMS_Game() :
    _isRunning(false),
    _currentState(tms::GameState::GAME),
    _windowWidth(0),
    _windowHeight(0)
{
}

tms::GameState TMS_Game::loadGame(const int winW, const int winH)
{
    /* Set width and height of the window. */
    _windowWidth = winW;
    _windowHeight = winH;

    /* Load textures. */
    if (!_textures.loadResources(tms::texture::LIST_FILE))
    {
        printf("Unable to load game textures.\n");
        return tms::GameState::EXIT;
    }

    /* Load shaders. */
    if (!_shaders.loadResources(tms::shader::LIST_FILE))
    {
        printf("Unable to load game shaders.\n");
        return tms::GameState::EXIT;
    }

    /* Load initial entities. */
    /* Load background. */
    std::vector<std::shared_ptr<TMS_Shader>> requiredShaders;
    for (int i = 0; i < static_cast<int>(TMS_Background::Shader::TOT); ++i)
    {
        requiredShaders.push_back(_shaders.get(TMS_Background::REQUIRED_SHADERS[i]));
    }
    std::vector<std::shared_ptr<TMS_Texture>> requiredTextures;
    for (int i = 0; i < static_cast<int>(TMS_Background::Texture::TOT); ++i)
    {
        requiredTextures.push_back((_textures.get(TMS_Background::REQUIRED_TEXTURES[i])));
    }
    
    tms::Rect backgroundSpan = { -_windowWidth, 0, BASE_WIDTH * _windowWidth, BASE_HEIGHT * _windowHeight };
    _entities.push_back(std::make_unique<TMS_Background>(
                        TMS_Background(requiredShaders, 
                                       requiredTextures,
                                       _entities.size(),
                                       backgroundSpan,
                                       _windowWidth,
                                       _windowHeight)));

    /* Set up the camera. */
    _camera.setBoundaries({ backgroundSpan.x, backgroundSpan.x + backgroundSpan.w, backgroundSpan.y, backgroundSpan.y + backgroundSpan.h }, 
                          _windowWidth, 
                          _windowHeight);

    /* Create an orthographic projection matrix. */
    _orthoMat = glm::ortho(0.0f, static_cast<float>(_windowWidth), static_cast<float>(_windowHeight), 0.0f, 0.0f, -static_cast<float>(tms::Layer::MAX_LAYER));

    return tms::GameState::GAME;
}

tms::GameState TMS_Game::gameLoop(tms::window_t& window)
{
    /* Model matrix. */
    glm::mat4 modelMat = glm::mat4(1.0f);
    /* View matrix. */
    glm::mat4 viewMat = _camera.getView();

    /* Combine the visualisation matrices. */
    glm::mat4 visualMatrix = _orthoMat * viewMat * modelMat;

    /* Set initial shader uniforms. */
    auto shader = _shaders.get(tms::shader::NAMES[static_cast<int>(tms::shader::Name::PLAIN)]);
    shader->use();
    shader->setUniform(static_cast<int>(tms::shader::Plain::CAMERA_MATRIX), glm::value_ptr(visualMatrix));
    shader->setUniform(static_cast<int>(tms::shader::Plain::TEXTURE), static_cast<int>(tms::texture::Layer::LAYER_0));

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

    const Uint8* currentKeyState = SDL_GetKeyboardState(nullptr); // Get current keypresses.
    _moveCamera(currentKeyState);

    return tms::GameState::GAME;
}

void TMS_Game::render(tms::window_t& window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& entity : _entities)
    {
        entity->render();
    }

    SDL_GL_SwapWindow(window.get());
}

void TMS_Game::_moveCamera(const Uint8* currentKeyState)
{
    /* Camera controls. */
    bool moved = false;
    if (currentKeyState[SDL_SCANCODE_A])
    {
        moved = true;
        if (currentKeyState[SDL_SCANCODE_W]) _camera.move(TMS_Camera::Direction::L_UP);
        else if (currentKeyState[SDL_SCANCODE_S]) _camera.move(TMS_Camera::Direction::L_DOWN);
        else _camera.move(TMS_Camera::Direction::L_MID);
    }
    else if (currentKeyState[SDL_SCANCODE_D])
    {
        moved = true;
        if (currentKeyState[SDL_SCANCODE_W]) _camera.move(TMS_Camera::Direction::R_UP);
        else if (currentKeyState[SDL_SCANCODE_S]) _camera.move(TMS_Camera::Direction::R_DOWN);
        else _camera.move(TMS_Camera::Direction::R_MID);
    }
    else if (currentKeyState[SDL_SCANCODE_W])
    {
        moved = true;
        _camera.move(TMS_Camera::Direction::C_UP);
    }
    else if (currentKeyState[SDL_SCANCODE_S])
    {
        moved = true;
        _camera.move(TMS_Camera::Direction::C_DOWN);
    }

    /* Update the shaders. */
    if (moved)
    {
        auto shader = _shaders.get(tms::shader::NAMES[static_cast<int>(tms::shader::Name::PLAIN)]);
        shader->use();
        shader->setUniform(static_cast<int>(tms::shader::Plain::CAMERA_MATRIX), glm::value_ptr(_orthoMat * _camera.getView()));
    }
}
