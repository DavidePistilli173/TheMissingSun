#include "TMS_Background.hpp"
#include "TMS_Game.hpp"


TMS_Game::TMS_Game() :
    _isRunning(false),
    _currentState(tms::GameState::GAME)
{
}

tms::GameState TMS_Game::loadGame(const int winW, const int winH)
{
    /* Load textures. */
    try
    {
        for (int i = 0; i < static_cast<int>(tms::texture::Name::TOT); ++i)
        {
            _textures.insert(std::pair<std::string, std::shared_ptr<TMS_Texture>>(
                             tms::texture::NAMES[i], 
                             std::make_shared<TMS_Texture>(TMS_Texture(tms::texture::FILES[i]))));
        }
    }
    catch (std::string e)
    {
        printf("Unable to load game textures:\n%s", e.c_str());
        return tms::GameState::EXIT;
    }

    /* Load shaders. */
    try
    {
        for (int i = 0; i < static_cast<int>(tms::shader::Name::TOT); ++i)
        {
            _shaders.insert(std::pair<std::string, std::shared_ptr<TMS_Shader>>(
                            tms::shader::NAMES[i],
                            std::make_shared<TMS_Shader>(TMS_Shader(tms::shader::FILES[2 * i], tms::shader::FILES[2 * i + 1]))));
        }
    }
    catch (std::string e)
    {
        printf("Unable to load game shaders:\n%s", e.c_str());
        return tms::GameState::EXIT;
    }

    /* Load initial entities. */
    /* Load background. */
    std::vector<std::shared_ptr<TMS_Shader>> requiredShaders;
    for (int i = 0; i < static_cast<int>(TMS_Background::Shader::TOT); ++i)
    {
        requiredShaders.push_back((*_shaders.find(TMS_Background::REQUIRED_SHADERS[i])).second);
    }
    std::vector<std::shared_ptr<TMS_Texture>> requiredTextures;
    for (int i = 0; i < static_cast<int>(TMS_Background::Texture::TOT); ++i)
    {
        requiredTextures.push_back((*_textures.find(TMS_Background::REQUIRED_TEXTURES[i])).second);
    }
    
    _entities.push_back(std::make_unique<TMS_Background>(
                        TMS_Background(requiredShaders, 
                                       requiredTextures,
                                       _entities.size(),
                                       tms::Rect({ -winW, 0, BASE_WIDTH * winW, BASE_HEIGHT * winH }))));

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
