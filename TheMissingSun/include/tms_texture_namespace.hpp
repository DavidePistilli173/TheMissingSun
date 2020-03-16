#ifndef TMS_TEXTURE_NAMESPACE_HPP
#define TMS_TEXTURE_NAMESPACE_HPP

#include "tms.hpp"

namespace tms::texture
{
    /* Texture locations for the same shader. */
    enum class Layer
    {
        LAYER_0
    };
    /***************** SETTINGS *****************/
    constexpr GLenum DEFAULT_X_WRAP = GL_REPEAT;
    constexpr GLenum DEFAULT_Y_WRAP = GL_REPEAT;
    constexpr GLenum DEFAULT_MIN_FILTER = GL_NEAREST_MIPMAP_LINEAR;
    constexpr GLenum DEFAULT_MAG_FILTER = GL_LINEAR;

    /***************** RESOURCE LIST *****************/
    const std::string LIST_FILE = "./config/textures.xml";
    constexpr char LIST_ROOT[] = "Textures";
    constexpr char LIST_ELEMENT[] = "Texture";
    constexpr char LIST_ELEMENT_NAME[] = "Name";
    constexpr char LIST_ELEMENT_FILE[] = "File";

    /***************** MENU *****************/
    const std::string TEST = "./resources/textures/test/container.jpg";
    const std::string MENU_BACKGROUND = "./resources/textures/main_menu/background.png";
    const std::string MENU_BUTTON = "./resources/textures/main_menu/button.png";

    /***************** GAME *****************/
    /* List of all game textures. */
    enum class Name
    {
        TEST,
        SKY,
        SURFACE,
        UNDERGROUND,
        BUILD_DEFAULT,
        ITEM_DEFAULT,
        UI_LIST_BACKGROUND,
        UI_ELEMENT,
        UI_SCROLL_BACKGROUND,
        UI_SCROLL_BAR,
        TOT
    };
    /* List of texture names. */
    const std::string NAMES[] =
    {
        "test",
        "sky",
        "surface",
        "underground",
        "build_default",
        "item_default",
        "ui_lst_bckgrnd",
        "ui_elm",
        "ui_scrll_bckgrnd",
        "ui_scrll_bar"
    };
}

#endif