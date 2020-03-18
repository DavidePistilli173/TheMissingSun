#include "../include/tms_shader_namespace.hpp"
#include "TMS_Button.hpp"

TMS_Button::TMS_Button() :
    label(""),
    vao(0), vbo(0), ebo(0),
    labelVAO(0), labelVBO(0),
    _defaultBackRect({0.0f,0.0f,0.0f,0.0f}),
    _currentBackRect({0.0f,0.0f,0.0f,0.0f}),
    _labelRect({0.0f,0.0f,0.0f,0.0f}),
    _labelColour({0,0,0,0}),
    _labelLenFraction(0)
{
    genRenderingBuffers();
}

TMS_Button::~TMS_Button()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &labelVAO);
    glDeleteBuffers(1, &labelVBO);
}

TMS_Button::TMS_Button(TMS_Button&& oldButton) noexcept
{
    label = std::move(oldButton.label);
    vao = oldButton.vao;
    oldButton.vao = 0;
    vbo = oldButton.vbo;
    oldButton.vbo = 0;
    ebo = oldButton.ebo;
    oldButton.ebo = 0;
    labelVAO = oldButton.labelVAO;
    oldButton.labelVAO = 0;
    labelVBO = oldButton.labelVBO;
    oldButton.labelVBO = 0;
    labelTexture = std::move(oldButton.labelTexture);

    _defaultBackRect = oldButton._defaultBackRect;
    _currentBackRect = oldButton._currentBackRect;
    _labelRect = oldButton._labelRect;
    _labelLenFraction = oldButton._labelLenFraction;
}

TMS_Button& TMS_Button::operator=(TMS_Button&& oldButton) noexcept
{
    if (this != &oldButton)
    {
        label = std::move(oldButton.label);
        vao = oldButton.vao;
        oldButton.vao = 0;
        vbo = oldButton.vbo;
        oldButton.vbo = 0;
        ebo = oldButton.ebo;
        oldButton.ebo = 0;
        labelVAO = oldButton.labelVAO;
        oldButton.labelVAO = 0;
        labelVBO = oldButton.labelVBO;
        oldButton.labelVBO = 0;
        labelTexture = std::move(oldButton.labelTexture);

        _defaultBackRect = oldButton._defaultBackRect;
        _currentBackRect = oldButton._currentBackRect;
        _labelRect = oldButton._labelRect;
        _labelLenFraction = oldButton._labelLenFraction;
    }
    return *this;
}

void TMS_Button::setDefaultX(const float x) { _defaultBackRect.x = x; }

void TMS_Button::setDefaultY(const float y) { _defaultBackRect.y = y; }

void TMS_Button::setDefaultW(const float w) { _defaultBackRect.w = w; }

void TMS_Button::setDefaultH(const float h) { _defaultBackRect.h = h; }

void TMS_Button::setX(const float x)
{
    _currentBackRect.x = x;
    float horizontalMargin = HORIZONTAL_BORDER * _currentBackRect.w;
    _labelRect.x = x + horizontalMargin + _labelLenFraction * (_currentBackRect.w - horizontalMargin)/2;

    setRenderingBuffers();
}

void TMS_Button::setY(const float y)
{
    _currentBackRect.y = y;
    _labelRect.y = y + VERTICAL_BORDER * _currentBackRect.h;

    setRenderingBuffers();
}

void TMS_Button::setW(const float w)
{
    _currentBackRect.w = w;
    float horizontalMargin = HORIZONTAL_BORDER * w;
    _labelRect.w = w - 2 * horizontalMargin - 2 * _labelLenFraction * (w - horizontalMargin)/2;

    setRenderingBuffers();
}

void TMS_Button::setH(const float h)
{
    _currentBackRect.h = h;
    _labelRect.h = h - 2 * VERTICAL_BORDER * h;

    setRenderingBuffers();
}

void TMS_Button::setColour(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a)
{
    _labelColour.r = r;
    _labelColour.g = g;
    _labelColour.b = b;
    _labelColour.a = a;
}

float TMS_Button::getDefaultX() const { return _defaultBackRect.x; }

float TMS_Button::getDefaultY() const { return _defaultBackRect.y; }

float TMS_Button::getDefaultW() const { return _defaultBackRect.w; }

float TMS_Button::getDefaultH() const { return _defaultBackRect.h; }

float TMS_Button::getX() const { return _currentBackRect.x; }

float TMS_Button::getY() const { return _currentBackRect.y; }

float TMS_Button::getW() const { return _currentBackRect.w; }

float TMS_Button::getH() const { return _currentBackRect.h; }

SDL_Color TMS_Button::getColour()
{
    return _labelColour;
}

void TMS_Button::resetToDefault()
{
    setX(_defaultBackRect.x);
    setY(_defaultBackRect.y);
    setW(_defaultBackRect.w);
    setH(_defaultBackRect.h);
}

void TMS_Button::genRenderingBuffers()
{
    /* Button background buffers. */
    /* VAO. */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    /* VBO. */
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    /* EBO. */
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Button label buffers. */
    /* VAO. */
    glGenVertexArrays(1, &labelVAO);
    glBindVertexArray(labelVAO);
    /* VBO. */
    glGenBuffers(1, &labelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, labelVBO);
    /* EBO. */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    setRenderingBuffers();
}

void TMS_Button::setRenderingBuffers()
{
    /* Button background buffers. */
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    float incrementedX = getX() + getW();
    float incrementedY = getY() + getH();

    float buttonVertexData[] =
    {
        getX(),         getY(),         static_cast<float>(tms::Layer::LAYER_7), 0.0f, 0.0f, // Top left corner.
        incrementedX,   getY(),         static_cast<float>(tms::Layer::LAYER_7), 1.0f, 0.0f, // Top right corner.
        incrementedX,   incrementedY,   static_cast<float>(tms::Layer::LAYER_7), 1.0f, 1.0f, // Bottom right corner.
        getX(),         incrementedY,   static_cast<float>(tms::Layer::LAYER_7), 0.0f, 1.0f // Bottom left corner.
    };

    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS), 3, GL_FLOAT, GL_FALSE, STRIDE_SIZE, 0);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS), 2, GL_FLOAT, GL_FALSE, STRIDE_SIZE, reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS));

    glBufferData(GL_ARRAY_BUFFER, sizeof(buttonVertexData), buttonVertexData, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(buttonVertexSequence), buttonVertexSequence, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Button label buffers. */
    glBindVertexArray(labelVAO);
    glBindBuffer(GL_ARRAY_BUFFER, labelVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    incrementedX = _labelRect.x + _labelRect.w;
    incrementedY = _labelRect.y + _labelRect.h;
    float labelVertexData[] =
    {
        _labelRect.x, _labelRect.y, static_cast<float>(tms::Layer::LAYER_6), 0.0f, 0.0f, // Top left corner.
        incrementedX, _labelRect.y, static_cast<float>(tms::Layer::LAYER_6), 1.0f, 0.0f, // Top right corner.
        incrementedX, incrementedY, static_cast<float>(tms::Layer::LAYER_6), 1.0f, 1.0f, // Bottom right corner.
        _labelRect.x, incrementedY, static_cast<float>(tms::Layer::LAYER_6), 0.0f, 1.0f // Bottom left corner.
    };

    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS), 3, GL_FLOAT, GL_FALSE, STRIDE_SIZE, 0);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS), 2, GL_FLOAT, GL_FALSE, STRIDE_SIZE, reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS));

    glBufferData(GL_ARRAY_BUFFER, sizeof(labelVertexData), labelVertexData, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool TMS_Button::setLabelTexture(tms::font_t& font)
{
    try
    {
        labelTexture = TMS_Texture(label, _labelColour, font);
    }
    catch (std::string error)
    {
        printf("%s", error.c_str());
        return false;
    }

    _labelLenFraction = 1.0f - labelTexture.getW() / MAX_TEXT_LEN;
    
    return true;
}

bool TMS_Button::checkCollision(const float x, const float y) const
{
    if (x < _currentBackRect.x || x > _currentBackRect.x + _currentBackRect.w) return false;
    if (y < _currentBackRect.y || y > _currentBackRect.y + _currentBackRect.h) return false;
    return true;
}
