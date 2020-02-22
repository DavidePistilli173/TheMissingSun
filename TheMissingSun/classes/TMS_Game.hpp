#ifndef TMS_GAME_HPP
#define TMS_GAME_HPP

#include <condition_variable>
#include <mutex>

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
    tms::GameState loadGameLogic(); // Load a game if there is not one running.
    void loadOpenGLAssets(); // Load all OpenGL related assets.
    tms::GameState gameLoop(); // Main game loop.
    void renderLoop(tms::window_t& window); // Rendering loop.

private:
    /***************** METHODS *****************/
    bool _mainCanLoad();  // Returns true if the main thread can load the game.
    bool _rendererCanLoad(); // Returns true if the renderer thread can load the game.

    /***************** VARIABLES *****************/
    bool _isRunning; // True if there already is a game running.

    /* Thread synchronisation variables. */
    LoadingState _loadingState; // Synchronises game loading between threads.
    std::mutex _mutex;
    std::condition_variable _waitCondition;

    tms::GameState _currentState; // Current game state.
    TMS_Clock _clock; // Game clock.

    std::shared_ptr<TMS_Texture> _textures[static_cast<int>(tms::texture::Name::TOT)]; // Array containing all game textures.
};

#endif