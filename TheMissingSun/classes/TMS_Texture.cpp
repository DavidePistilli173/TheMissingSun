#include <memory>
#include <SDL.h>
#include <SDL_image.h>

#include "../include/tms.hpp"
#include "../include/tms_texture_namespace.hpp"
#include "TMS_Texture.hpp"

TMS_Texture::TMS_Texture() :
    _id(0),
    _wrapX(tms::texture::DEFAULT_X_WRAP),
    _wrapY(tms::texture::DEFAULT_Y_WRAP),
    _minFilter(tms::texture::DEFAULT_MIN_FILTER),
    _magFilter(tms::texture::DEFAULT_MAG_FILTER),
    _width(0),
    _height(0)
{
}

TMS_Texture::TMS_Texture(const std::string fileName)
{
    /* Load the file into an SDL surface. */
    tms::surface_t textureSurface(
        IMG_Load(fileName.c_str()),
        [](SDL_Surface* surf) { SDL_FreeSurface(surf); }
    );
    if (textureSurface == nullptr) throw "Failed to load texture " + fileName + "\n " + IMG_GetError();
    _loadTexture(textureSurface, DataFormat::PNG_JPG);
}

TMS_Texture::TMS_Texture(const std::string text, const SDL_Color textColour, const tms::font_t& font)
{
    tms::surface_t textureSurface(
        TTF_RenderUTF8_Blended(font.get(), text.c_str(), textColour),
        [](SDL_Surface* surf) { SDL_FreeSurface(surf); }
    );
    if (textureSurface == nullptr) throw "Failed to generate texture from text " + text + "\n " + TTF_GetError();
    _loadTexture(textureSurface, DataFormat::BLENDED_TEXT);
}

TMS_Texture::TMS_Texture(TMS_Texture&& oldTexture) noexcept
{
    /* Move texture id. */
    _id = oldTexture._id;
    oldTexture._id = 0;

    /* Copy texture settings. */
    _wrapX = oldTexture._wrapX;
    _wrapY = oldTexture._wrapY;
    _minFilter = oldTexture._minFilter;
    _magFilter = oldTexture._magFilter;

    /* Copy texture dimensions. */
    _width = oldTexture._width;
    oldTexture._width = 0;
    _height = oldTexture._height;
    oldTexture._height = 0;
}

TMS_Texture& TMS_Texture::operator=(TMS_Texture&& oldTexture) noexcept
{
    if (&oldTexture != this)
    {
        /* Move texture id. */
        _id = oldTexture._id;
        oldTexture._id = 0;

        /* Move texture settings. */
        _wrapX = oldTexture._wrapX;
        _wrapY = oldTexture._wrapY;
        _minFilter = oldTexture._minFilter;
        _magFilter = oldTexture._magFilter;

        /* Copy texture dimensions. */
        _width = oldTexture._width;
        oldTexture._width = 0;
        _height = oldTexture._height;
        oldTexture._height = 0;
    }
    return *this;
}

int TMS_Texture::getW() const
{
    return _width;
}

int TMS_Texture::getH() const
{
    return _height;
}

void TMS_Texture::setWrapX(const GLenum setting)
{
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, setting);
}

void TMS_Texture::setWrapY(const GLenum setting)
{
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, setting);
}

void TMS_Texture::_loadTexture(tms::surface_t& textureSurface, DataFormat format)
{
    /* Store texture dimensions. */
    _width = textureSurface->w;
    _height = textureSurface->h;

    /* Generate a new OpenGL texture. */
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    /* Set default texture settings. */
    _wrapX = tms::texture::DEFAULT_X_WRAP;
    _wrapY = tms::texture::DEFAULT_Y_WRAP;
    _minFilter = tms::texture::DEFAULT_MIN_FILTER;
    _magFilter = tms::texture::DEFAULT_MAG_FILTER;

    /* Set the behaviour when outside texture coordinates. */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tms::texture::DEFAULT_X_WRAP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tms::texture::DEFAULT_Y_WRAP);
    /* Set the texel selection/interpolation crtiteria. */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tms::texture::DEFAULT_MIN_FILTER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tms::texture::DEFAULT_MAG_FILTER);

    /* Retrieve the image format. */
    int imageFormat = GL_RGB;
    int pixelFormat = GL_UNSIGNED_BYTE;

    switch (format)
    {
    case DataFormat::PNG_JPG:
        if (textureSurface->format->BytesPerPixel == 4) imageFormat = GL_RGBA;
        break;
    case DataFormat::BLENDED_TEXT:
        imageFormat = GL_RGBA;
        textureSurface.reset(SDL_ConvertSurfaceFormat(textureSurface.get(), SDL_PIXELFORMAT_ABGR8888, 0));
        break;
    }

    /* Load the texture into memory. */
    glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, textureSurface->w, textureSurface->h, 0, imageFormat, pixelFormat, textureSurface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}
