#ifndef TMS_HPP
#define TMS_HPP

#include <memory>
#include <SDL.h>
#include <string>

#include "glad/glad.h"

namespace tms
{
    /***************** TYPE ALIASES *****************/
    using window_t = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>; // Game window.
    using file_t = std::unique_ptr<SDL_RWops, void(*)(SDL_RWops*)>; // File handle.
    using surface_t = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>; // SDL_Surface image.

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
    /* Times. */
    constexpr int MENU_UPDATE_TIME = 17; // Approximately 60 UPS.
    /* Rendering matrices. */
    /* View matrix. */
    constexpr float DEFAULT_VIEW_X = 0.0f;
    constexpr float DEFAULT_VIEW_Y = 0.0f;
    constexpr float DEFAULT_VIEW_Z = -5.0f;
    /* Orthographic projection matrix. */
    constexpr int MIN_RENDER_LAYER = -5; // Closest rendering layer.
    constexpr int DEFAULT_RENDER_LAYER = 0; // Default rendering layer.
    /* Shaders. */
    constexpr int UNIFORM_NAME_LEN = 32; // Max length of a uniform name.
    constexpr int CODE_READ_CHUNK = 4096; // Size of a single code chunk to be read.
    constexpr int INIT_CODE_SIZE = 32768; // Initial size of the strings that contain the code.
    constexpr int SHADER_LOG_SIZE = 2048; // Size of the shader compilation error log.
    /* Textures. */
    constexpr GLenum DEFAULT_X_WRAP = GL_REPEAT;
    constexpr GLenum DEFAULT_Y_WRAP = GL_REPEAT;
    constexpr GLenum DEFAULT_MIN_FILTER = GL_NEAREST_MIPMAP_LINEAR;
    constexpr GLenum DEFAULT_MAG_FILTER = GL_LINEAR;

    /***************** CONFIGURATION FILES *****************/
    /* Main menu. */
    constexpr char CONFIG_MM_NAME[] = "config/main_menu.xml"; // Configuration file name.
    constexpr char CONFIG_MM_TAG_ROOT[] = "Menu"; // Root tag.
    constexpr char CONFIG_MM_TAG_PAGE[] = "MenuPage"; // Menu page tag.
    constexpr char CONFIG_MM_TAG_ID[] = "Id"; // Page id tag.
    constexpr char CONFIG_MM_TAG_TITLE[] = "Title"; // Page title tag.
    constexpr char CONFIG_MM_TAG_BUTTONS[] = "Buttons"; // Button list tag.
    constexpr char CONFIG_MM_TAG_BUTTON[] = "Button"; // Button tag.
    constexpr char CONFIG_MM_TAG_LABEL[] = "Label"; // Button label tag. 
    constexpr char CONFIG_MM_TAG_DEST[] = "Destination"; // Button destination page tag.
    constexpr char CONFIG_MM_TAG_X[] = "X"; // Button x coordinate tag.
    constexpr char CONFIG_MM_TAG_Y[] = "Y"; // Button y coordinate tag.
    constexpr char CONFIG_MM_TAG_WIDTH[] = "Width"; // Button width tag. 
    constexpr char CONFIG_MM_TAG_HEIGHT[] = "Height"; // Button height tag.

    /***************** ENUMS *****************/
    /* List of all possible game states. */
    enum class GameState
    {
        MENU,
        LOADING,
        GAME,
        EXIT
    };
    /* List of rendering layers. */
    enum class Layer
    {
        LAYER_0 = MIN_RENDER_LAYER,
        LAYER_1,
        LAYER_2,
        LAYER_3,
        LAYER_4,
        LAYER_5,
        LAYER_6,
        LAYER_7,
        MAX_LAYER
    };

    /***************** CONSTEXPR FUNCTIONS *****************/
    /* Return the default rendering layer. */
    constexpr Layer default_layer();

    /***************** FUNCTIONS *****************/
    /* Copy the source string into the destination one. Maximum string length: UNIFORM_NAME_LEN. */
    void stringCopy(char* destination, const char* source);
}

#endif