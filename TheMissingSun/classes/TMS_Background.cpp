#include "../include/tms_shader_namespace.hpp"
#include "TMS_Background.hpp"

/* Build a two-level background: first level has the sky/surface, the second one has the underground. */
TMS_Background::TMS_Background(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures,
                               tms::Rect span) :
    TMS_Entity(shaders, textures),
    _skyRect({span.x, 
              span.y, 
              span.w, 
              static_cast<int>(span.h * LEVEL_HEIGHT_FRACTION)}),
    _surfaceRect({span.x, 
                  span.y, 
                  span.w, 
                  static_cast<int>(span.h * LEVEL_HEIGHT_FRACTION)}),
    _undergroundRect({span.x, 
                      span.y + static_cast<int>(span.h * LEVEL_HEIGHT_FRACTION), 
                      span.w, 
                      static_cast<int>(span.h * (1 - LEVEL_HEIGHT_FRACTION))}),
    _skyVAO(0), _skyVBO(0), _surfaceVAO(0), _surfaceVBO(0), _undergroundVAO(0), _undergroundVBO(0), _EBO(0)
{
    if (_shaders.size() != static_cast<int>(Shader::TOT)) throw "Wrong number of shaders for background initialisation.\n";
    if (_textures.size() != static_cast<int>(Texture::TOT)) throw "Wrong number of textures for background initialisation.\n";
}

TMS_Background::TMS_Background(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures, 
                               unsigned int id, tms::Rect span) :
    TMS_Entity(shaders, textures, id),
    _skyRect({ span.x,
              span.y,
              span.w,
              static_cast<int>(span.h* LEVEL_HEIGHT_FRACTION) }),
    _surfaceRect({ span.x,
                  span.y,
                  span.w,
                  static_cast<int>(span.h* LEVEL_HEIGHT_FRACTION) }),
    _undergroundRect({ span.x,
                      span.y + static_cast<int>(span.h* LEVEL_HEIGHT_FRACTION),
                      span.w,
                      static_cast<int>(span.h* (1 - LEVEL_HEIGHT_FRACTION)) }),
    _skyVAO(0), _skyVBO(0), _surfaceVAO(0), _surfaceVBO(0), _undergroundVAO(0), _undergroundVBO(0), _EBO(0)
{
    if (_shaders.size() != static_cast<int>(Shader::TOT)) throw "Wrong number of shaders for background initialisation.\n";
    if (_textures.size() != static_cast<int>(Texture::TOT)) throw "Wrong number of textures for background initialisation.\n";
}

TMS_Background::~TMS_Background()
{
    glDeleteBuffers(1, &_skyVAO);
    glDeleteBuffers(1, &_skyVBO);
    glDeleteBuffers(1, &_surfaceVAO);
    glDeleteBuffers(1, &_surfaceVBO);
    glDeleteBuffers(1, &_undergroundVAO);
    glDeleteBuffers(1, &_undergroundVBO);
    glDeleteBuffers(1, &_EBO);
}

TMS_Background::TMS_Background(TMS_Background&& oldBackground) noexcept :
    TMS_Entity(std::move(oldBackground)),
    _skyRect(oldBackground._skyRect),
    _surfaceRect(oldBackground._surfaceRect),
    _undergroundRect(oldBackground._undergroundRect)
{
        _EBO = oldBackground._EBO;
        oldBackground._EBO = 0;
        _skyVAO = oldBackground._skyVAO;
        oldBackground._skyVAO = 0;
        _skyVBO = oldBackground._skyVBO;
        oldBackground._skyVBO = 0;
        _surfaceVAO = oldBackground._surfaceVAO;
        oldBackground._surfaceVAO = 0;
        _surfaceVBO = oldBackground._surfaceVBO;
        oldBackground._surfaceVBO = 0;
        _undergroundVAO = oldBackground._undergroundVAO;
        oldBackground._undergroundVAO = 0;
        _undergroundVBO = oldBackground._undergroundVBO;
        oldBackground._undergroundVBO = 0;
}

TMS_Background& TMS_Background::operator=(TMS_Background&& oldBackground) noexcept
{
    if (this != &oldBackground)
    {
        TMS_Entity::operator=(std::move(oldBackground));

        _skyRect = oldBackground._skyRect;
        _surfaceRect = oldBackground._surfaceRect;
        _undergroundRect = oldBackground._undergroundRect;

        _EBO = oldBackground._EBO;
        oldBackground._EBO = 0;
        _skyVAO = oldBackground._skyVAO;
        oldBackground._skyVAO = 0;
        _skyVBO = oldBackground._skyVBO;
        oldBackground._skyVBO = 0;
        _surfaceVAO = oldBackground._surfaceVAO;
        oldBackground._surfaceVAO = 0;
        _surfaceVBO = oldBackground._surfaceVBO;
        oldBackground._surfaceVBO = 0;
        _undergroundVAO = oldBackground._undergroundVAO;
        oldBackground._undergroundVAO = 0;
        _undergroundVBO = oldBackground._undergroundVBO;
        oldBackground._undergroundVBO = 0;
    }
    return *this;
}

tms::Layer TMS_Background::getLayer()
{
    return tms::Layer::BACKGROUND_LAYER;
}

bool TMS_Background::checkCollision(const int x, const int y)
{
    return true;
}

std::optional<TMS_Action> TMS_Background::handleEvent(const TMS_Event& event)
{
    return std::optional<TMS_Action>();
}

void TMS_Background::render()
{
    _shaders[static_cast<int>(Shader::PLAIN)]->use();

    /* Draw the sky. */
    _textures[static_cast<int>(Texture::SKY)]->bind();
    glBindVertexArray(_skyVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    /* Draw the surface. */
    _textures[static_cast<int>(Texture::SURFACE)]->bind();
    glBindVertexArray(_surfaceVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    /* Draw the underground. */
    _textures[static_cast<int>(Texture::UNDERGROUND)]->bind();
    glBindVertexArray(_undergroundVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void TMS_Background::initBuffers()
{
    /* Create the EBO. */
    glGenBuffers(1, &_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);

    float eboData[] =
    {
        0, 1, 2,
        2, 3, 0
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eboData), eboData, GL_STATIC_DRAW);

    /* Initialise the sky. */
    glGenVertexArrays(1, &_skyVAO);
    glBindVertexArray(_skyVAO);
    glGenBuffers(1, &_skyVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _skyVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);

    float maxX = static_cast<float>(_skyRect.x) + static_cast<float>(_skyRect.w);
    float maxY = static_cast<float>(_skyRect.y) + static_cast<float>(_skyRect.h);
    float maxTexW = static_cast<float>(_skyRect.w) / _textures[static_cast<int>(Texture::SKY)]->getW();
    float maxTexH = static_cast<float>(_skyRect.h) / _textures[static_cast<int>(Texture::SKY)]->getW();
    float data[BUFFER_SIZE];
    // Top left corner.
    data[0] = static_cast<float>(_skyRect.x);
    data[1] = static_cast<float>(_skyRect.y);
    data[2] = static_cast<int>(tms::Layer::BACKGROUND_LAYER);
    data[3] = 0.0f;
    data[4] = 0.0f;
    // Top right corner.
    data[5] = maxX;
    data[6] = static_cast<float>(_skyRect.y);
    data[7] = static_cast<int>(tms::Layer::BACKGROUND_LAYER);
    data[8] = maxTexW;
    data[9] = 0.0f;
    // Bottom right corner
    data[10] = maxX;
    data[11] = maxY;
    data[12] = static_cast<int>(tms::Layer::BACKGROUND_LAYER);
    data[13] = maxTexW;
    data[14] = maxTexH;
    // Bottom left corner.
    data[15] = static_cast<float>(_skyRect.x);
    data[16] = maxY;
    data[17] = static_cast<int>(tms::Layer::BACKGROUND_LAYER);
    data[18] = 0.0f;
    data[19] = maxTexH;

    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    int strideSize = 5 * sizeof(float);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS), 3, GL_FLOAT, GL_FALSE, strideSize, 0);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS), 2, GL_FLOAT, GL_FALSE, strideSize, reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS));
    glBindVertexArray(0);

    /* Initialise the surface. */
    glGenVertexArrays(1, &_surfaceVAO);
    glBindVertexArray(_surfaceVAO);
    glGenBuffers(1, &_surfaceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _surfaceVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);

    maxX = static_cast<float>(_surfaceRect.x) + static_cast<float>(_surfaceRect.w);
    maxY = static_cast<float>(_surfaceRect.y) + static_cast<float>(_surfaceRect.h);
    maxTexW = static_cast<float>(_surfaceRect.w) / _textures[static_cast<int>(Texture::SURFACE)]->getW();
    maxTexH = static_cast<float>(_surfaceRect.h) / _textures[static_cast<int>(Texture::SURFACE)]->getW();
    // Top left corner.
    data[0] = static_cast<float>(_surfaceRect.x);
    data[1] = static_cast<float>(_surfaceRect.y);
    // Top right corner.
    data[5] = maxX;
    data[6] = static_cast<float>(_surfaceRect.y);
    data[8] = maxTexW;
    // Bottom right corner
    data[10] = maxX;
    data[11] = maxY;
    data[13] = maxTexW;
    data[14] = maxTexH;
    // Bottom left corner.
    data[15] = static_cast<float>(_surfaceRect.x);
    data[16] = maxY;
    data[19] = maxTexH;

    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS), 3, GL_FLOAT, GL_FALSE, strideSize, 0);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS), 2, GL_FLOAT, GL_FALSE, strideSize, reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS));
    glBindVertexArray(0);

    /* Initialise the underground. */
    glGenVertexArrays(1, &_undergroundVAO);
    glBindVertexArray(_undergroundVAO);
    glGenBuffers(1, &_undergroundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _undergroundVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);

    maxX = static_cast<float>(_undergroundRect.x) + static_cast<float>(_undergroundRect.w);
    maxY = static_cast<float>(_undergroundRect.y) + static_cast<float>(_undergroundRect.h);
    maxTexW = static_cast<float>(_undergroundRect.w) / _textures[static_cast<int>(Texture::UNDERGROUND)]->getW();
    maxTexH = static_cast<float>(_undergroundRect.h) / _textures[static_cast<int>(Texture::UNDERGROUND)]->getW();
    // Top left corner.
    data[0] = static_cast<float>(_undergroundRect.x);
    data[1] = static_cast<float>(_undergroundRect.y);
    // Top right corner.
    data[5] = maxX;
    data[6] = static_cast<float>(_undergroundRect.y);
    data[8] = maxTexW;
    // Bottom right corner
    data[10] = maxX;
    data[11] = maxY;
    data[13] = maxTexW;
    data[14] = maxTexH;
    // Bottom left corner.
    data[15] = static_cast<float>(_undergroundRect.x);
    data[16] = maxY;
    data[19] = maxTexH;

    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS), 3, GL_FLOAT, GL_FALSE, strideSize, 0);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS), 2, GL_FLOAT, GL_FALSE, strideSize, reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
