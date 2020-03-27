#ifndef TMS_GAME_HPP
#define TMS_GAME_HPP

#include "../include/tms.hpp"
#include "../include/tms_shader_namespace.hpp"
#include "../include/tms_texture_namespace.hpp"
#include "./Entities/TMS_Background.hpp"
#include "./Entities/TMS_Entity.hpp"
#include "./Entities/TMS_PlayerBase.hpp"
#include "TMS_Camera.hpp"
#include "TMS_Clock.hpp"
#include "TMS_EventDispatcher.hpp"
#include "TMS_ResourceContainer.hpp"
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
    static constexpr int BACKGROUND_HEIGHT = 2; // Height of the background with respect to the window's height.
    static constexpr float BASE_HEIGHT = 1.6f; // Height of the base with respect to the window's height.

private:
    /***************** METHODS *****************/
    void _moveCamera(const Uint8* currentKeyState); // Update the position of the camera.

    /***************** VARIABLES *****************/
    bool _isRunning; // True if there already is a game running.

    tms::GameState _currentState; // Current game state.
    TMS_Clock _clock; // Game clock.
    TMS_Camera _camera; // Game camera.
    glm::mat4 _orthoMat; // Orthographic projection matrix.

    TMS_EventDispatcher _eventDispatcher; // Routes each event to the relevant entity.

    TMS_ResourceContainer<TMS_Shader> _shaders; // All game shaders.
    TMS_ResourceContainer<TMS_Texture> _textures; // All game textures.
    std::vector<TMS_Entity*> _entities; // Vector containing active game entities.

    std::unique_ptr<TMS_Background> _background; // Base background.
    std::unique_ptr<TMS_PlayerBase> _base; // Player base.

    int _windowWidth, _windowHeight; // Dimensions of the window.
};

#endif