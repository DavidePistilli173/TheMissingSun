#ifndef TMS_TEXTURE_HPP
#define TMS_TEXTURE_HPP

#include <string>

#include "../include/glad/glad.h"

class TMS_Texture
{
public:
    TMS_Texture(); // Create an empty texture.
    TMS_Texture(const std::string fileName); // Load a texture from file.
    ~TMS_Texture()
    {
        glDeleteTextures(1, &_id);
    }
private:
    unsigned int _id;
    GLenum _wrapX, _wrapY; // Wrapping settings for x and y axes.
    GLenum _minFilter, _magFilter; // Filter settings for scaling down and up.
};

#endif