#ifndef TMS_HPP
#define TMS_HPP

#include <memory>
#include <SDL.h>
#include <string>

namespace tms
{
    /***************** TYPE ALIASES *****************/
    /* Game window type. */
    using window_t = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;

    /***************** ENUMS *****************/
    /* List of all possible game states. */
    enum class GameState
    {
        MENU,
        LOADING,
        GAME,
        EXIT
    };
    /* List of all drawable classes. */
    enum class Drawable
    {
        TOT // Total number of elements.
    };

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
}

#endif