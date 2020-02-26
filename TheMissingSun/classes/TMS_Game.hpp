#ifndef TMS_GAME_HPP
#define TMS_GAME_HPP

#include "../include/tms.hpp"
#include "../include/tms_texture_namespace.hpp"
#include "TMS_Clock.hpp"
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
    tms::GameState loadGame(); // Load all game assets.
    tms::GameState gameLoop(tms::window_t& window); // Main game loop.
    tms::GameState handleEvents();
    void render(tms::window_t& window); // Render the game.

private:
    /***************** VARIABLES *****************/
    bool _isRunning; // True if there already is a game running.

    tms::GameState _currentState; // Current game state.
    TMS_Clock _clock; // Game clock.

    std::shared_ptr<TMS_Texture> _textures[static_cast<int>(tms::texture::Name::TOT)]; // Array containing all game textures.
};

#endif