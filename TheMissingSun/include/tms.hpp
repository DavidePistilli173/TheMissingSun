#ifndef TMS_HPP
#define TMS_HPP

#include <glm.hpp>
#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "glad/glad.h"

namespace tms
{
    /***************** TYPE ALIASES *****************/
    using window_t = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>; // Game window.
    using file_t = std::unique_ptr<SDL_RWops, void(*)(SDL_RWops*)>; // File handle.
    using surface_t = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>; // SDL_Surface image.
    using font_t = std::unique_ptr<TTF_Font, void(*)(TTF_Font*)>; // SDL true type font.

    /***************** STRUCTS *****************/
    /* Data for a rectangle. */
    template <typename T>
    union Rect
    {
        constexpr Rect() {}
        constexpr Rect(T a, T b, T c, T d) :
            x(a), y(b), w(c), h(d) {}

        /* Upper left corner and dimensions. */
        struct
        {
            T x, y; // x and y coordinates of the top-left corner.
            T w, h; // Width and height respectively.
        };
        /* Vertical and horizontal limits. */
        struct
        {
            T minX, maxX; // Left and right limits of the rectangle.
            T minY, maxY; // Upper and lower limits of the rectangle.
        };
    };

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
    /* Default buffer data. */
    constexpr unsigned int EBO_QUAD_DATA[] =
    {
        0,1,2,
        2,3,0
    };
    constexpr Rect<float> DEFAULT_TEX_COORDS = { 0.0f, 0.0f, 1.0f, 1.0f };

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
    constexpr int BASE_FONT_SIZE = 180;
    constexpr char TEST_FONT[] = "resources/fonts/test_font.ttf";

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
    /* Types of event relevant to the game. */
    enum class EventType
    {
        MOUSE_LEFT_CLICK,
        MOUSE_HOVER,
        KEY_PRESS,
        TOT
    };
    /* OpenGL error severities. */
    enum class ErrorSeverity
    {
        NOTIFICATION,
        LOW,
        MEDIUM,
        HIGH
    };

    /***************** CONSTEXPR FUNCTIONS *****************/
    /* Return the default rendering layer. */
    constexpr Layer default_layer()
    {
        return static_cast<Layer>(DEFAULT_RENDER_LAYER);
    }
    /* Return the number of rendering layers. */
    constexpr int layer_num()
    {
        return static_cast<int>(Layer::MIN_LAYER) - static_cast<int>(Layer::BACKGROUND_LAYER) + 1;
    }
    /* Convert the layer number to an array index (from MIN_LAYER to BACKGROUND_LAYER). */
    constexpr int layer_index(Layer layer)
    {
        return static_cast<int>(Layer::MIN_LAYER) - static_cast<int>(layer);
    }
    /* Convert a float in range [0,1] to an int in range [0,255]. */
    constexpr Uint8 to_SDL_colour(float colour)
    {
        return static_cast<Uint8>(colour * 255);
    }

    /***************** FUNCTIONS *****************/
    /* Copy the source string into the destination one. Maximum string length: UNIFORM_NAME_LEN. */
    void stringCopy(char* destination, const char* source);
    /* Print all OpenGL errors. */
    void GLAPIENTRY openGLError(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam);
}

#endif