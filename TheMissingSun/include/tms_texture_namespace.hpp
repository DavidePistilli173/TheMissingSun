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

    /***************** MENU *****************/
    const std::string TEST = "./resources/textures/test/container.jpg";
    const std::string MENU_BACKGROUND = "./resources/textures/main_menu/background.png";
    const std::string MENU_BUTTON = "./resources/textures/main_menu/button.png";
}

#endif