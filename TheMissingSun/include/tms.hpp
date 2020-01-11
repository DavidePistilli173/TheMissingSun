#ifndef TMS_HPP
#define TMS_HPP

#include <glm.hpp>
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
    /* Times. */
    constexpr int MENU_UPDATE_TIME = 17; // Approximately 60 UPS.
    /* Rendering matrices. */
    /* View matrix. */
    constexpr float DEFAULT_VIEW_X = 0.0f;
    constexpr float DEFAULT_VIEW_Y = 0.0f;
    constexpr float DEFAULT_VIEW_Z = 5.0f;
    const glm::vec3 DEFAULT_CAMERA_POSITION(DEFAULT_VIEW_X, DEFAULT_VIEW_Y, DEFAULT_VIEW_Z);
    const glm::vec3 DEFAULT_CAMERA_TARGET(0.0f, 0.0f, 0.0f);
    const glm::vec3 DEFAULT_CAMERA_UP(0.0f, 1.0f, 0.0f);
    /* Orthographic projection matrix. */
    constexpr int MAX_RENDER_LAYER = -10; // Farthest rendering layer.
    constexpr int DEFAULT_RENDER_LAYER = 5; // Default rendering layer.
    /* Shaders. */
    constexpr int UNIFORM_NAME_LEN = 32; // Max length of a uniform name.
    constexpr int CODE_READ_CHUNK = 4096; // Size of a single code chunk to be read.
    constexpr int INIT_CODE_SIZE = 32768; // Initial size of the strings that contain the code.
    constexpr int SHADER_LOG_SIZE = 2048; // Size of the shader compilation error log.
    /* Colours. */
    /* Default colour. */
    constexpr float DEFAULT_RED = 0.2f;
    constexpr float DEFAULT_GREEN = 0.2f;
    constexpr float DEFAULT_BLUE = 0.2f;
    /* White. */
    constexpr float COLOUR_WHITE_R = 1.0f;
    constexpr float COLOUR_WHITE_G = 1.0f;
    constexpr float COLOUR_WHITE_B = 1.0f;

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

    /***************** FONTS *****************/
    constexpr char BASE_FONT[] = "resources/fonts/base_font.ttf";
    constexpr int BASE_FONT_SIZE = 14;

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
        MAX_LAYER = MAX_RENDER_LAYER, // Parameter required for the orthographic projection matrix.
        BACKGROUND_LAYER = -4,
        LAYER_7,
        LAYER_6,
        LAYER_5,
        LAYER_4,
        LAYER_3,
        LAYER_2,
        LAYER_1,
        LAYER_0,
        MIN_LAYER
    };

    /***************** CONSTEXPR FUNCTIONS *****************/
    /* Return the default rendering layer. */
    constexpr Layer default_layer();
    constexpr int toSDLColour(float colour);

    /***************** FUNCTIONS *****************/
    /* Copy the source string into the destination one. Maximum string length: UNIFORM_NAME_LEN. */
    void stringCopy(char* destination, const char* source);
}

#endif