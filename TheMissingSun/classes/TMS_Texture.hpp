#ifndef TMS_TEXTURE_HPP
#define TMS_TEXTURE_HPP

#include <SDL.h>
#include <string>
#include <string_view>

#include "../include/glad/glad.h"
#include "../include/tms.hpp"

class TMS_Texture
{
public:
    /* Different SDL_Surface data formats. */
    enum class DataFormat
    {
        PNG_JPG,
        BLENDED_TEXT
    };

    TMS_Texture(); // Create an empty texture.
    TMS_Texture(const std::string_view fileName); // Load a texture from file.
    TMS_Texture(const std::string_view text, const SDL_Color textColour, const tms::font_t& font); // Generate a texture from text.
    ~TMS_Texture()
    {
        glDeleteTextures(1, &_id);
    }
    /* Disable texture copy. */
    TMS_Texture(const TMS_Texture& oldTexture) = delete;
    TMS_Texture& operator= (const TMS_Texture& oldTexture) = delete;
    /* Move a texture from one object to the other. */
    TMS_Texture(TMS_Texture&& oldTexture) noexcept;
    TMS_Texture& operator= (TMS_Texture&& oldTexture) noexcept;

    /**************** METHODS ****************/
    /* Getters. */
    int getW() const;
    int getH() const;
    /* Setters. */
    void setWrapX(const GLenum setting);
    void setWrapY(const GLenum setting);
    /* Bind the current texture. */
    void bind()
    {
        glBindTexture(GL_TEXTURE_2D, _id);
    }
private:
    /**************** METHODS ****************/
    void _loadTexture(tms::surface_t& textureSurface, DataFormat format);

    unsigned int _id;
    GLenum _wrapX, _wrapY; // Wrapping settings for x and y axes.
    GLenum _minFilter, _magFilter; // Filter settings for scaling down and up.

    unsigned int _width, _height; // Texture dimensions.
};

#endif