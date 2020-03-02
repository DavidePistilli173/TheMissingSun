#ifndef TMS_GAME_HPP
#define TMS_GAME_HPP

#include <map>

#include "../include/tms.hpp"
#include "../include/tms_shader_namespace.hpp"
#include "../include/tms_texture_namespace.hpp"
#include "TMS_Clock.hpp"
#include "TMS_Entity.hpp"
#include "TMS_Shader.hpp"
#include "TMS_Texture.hpp"

class TMS_Game
{
public:
    /* Different loading states. */
    enum class LoadingState
    {
        GL_ASSETS,
        MAIN_ASSETS,
        GL_BUFFERS,
        COMPLETE,
        FAILURE
    };

    TMS_Game();

    /***************** METHODS *****************/
    tms::GameState loadGame(const int winW, const int winH); // Load all game assets.
    tms::GameState gameLoop(tms::window_t& window); // Main game loop.
    tms::GameState handleEvents();
    void render(tms::window_t& window); // Render the game.

    /***************** CONSTANTS *****************/
    static constexpr int BASE_WIDTH = 3; // Width of the base with respect to the window's width.
    static constexpr int BASE_HEIGHT = 2; // Height of the base with respect to the window's height.

private:
    /***************** VARIABLES *****************/
    bool _isRunning; // True if there already is a game running.

    tms::GameState _currentState; // Current game state.
    TMS_Clock _clock; // Game clock.

    std::map<std::string, std::shared_ptr<TMS_Texture>> _textures; // All game textures.
    std::map<std::string, std::shared_ptr<TMS_Shader>> _shaders; // All game shaders.
    std::vector<std::unique_ptr<TMS_Entity>> _entities; // Vector containing active game entities.

    int _windowWidth, _windowHeight;
};

#endif