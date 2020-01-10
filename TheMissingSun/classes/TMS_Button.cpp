#include "../include/tms_shader_namespace.hpp"
#include "TMS_Button.hpp"

TMS_Button::TMS_Button() :
    label(""),
    vao(0), vbo(0), ebo(0),
    _defaultBackRect({0,0,0,0}),
    _currentBackRect({0,0,0,0}),
    _labelRect({0,0,0,0}),
    _modified(false)
{
}

TMS_Button::TMS_Button(TMS_Button&& oldButton) noexcept
{
    label = std::move(oldButton.label);
    vao = oldButton.vao;
    vbo = oldButton.vbo;
    ebo = oldButton.ebo;
    labelTexture = std::move(oldButton.labelTexture);

    _defaultBackRect = oldButton._defaultBackRect;
    _currentBackRect = oldButton._currentBackRect;
    _labelRect = oldButton._labelRect;
    _modified = oldButton._modified.load();
}

TMS_Button& TMS_Button::operator=(TMS_Button&& oldButton) noexcept
{
    if (this != &oldButton)
    {
        label = std::move(oldButton.label);
        vao = oldButton.vao;
        vbo = oldButton.vbo;
        ebo = oldButton.ebo;
        labelTexture = std::move(oldButton.labelTexture);

        _defaultBackRect = oldButton._defaultBackRect;
        _currentBackRect = oldButton._currentBackRect;
        _labelRect = oldButton._labelRect;
        _modified = oldButton._modified.load();
    }
    return *this;
}

void TMS_Button::setDefaultX(const int x) { _defaultBackRect.x = x; }

void TMS_Button::setDefaultY(const int y) { _defaultBackRect.y = y; }

void TMS_Button::setDefaultW(const int w) { _defaultBackRect.w = w; }

void TMS_Button::setDefaultH(const int h) { _defaultBackRect.h = h; }

void TMS_Button::setX(const int x)
{
    _currentBackRect.x = x;
    _labelRect.x = x + HORIZONTAL_BORDER * _currentBackRect.w;
    _modified = true;
}

void TMS_Button::setY(const int y)
{
    _currentBackRect.y = y;
    _labelRect.y = y + VERTICAL_BORDER * _currentBackRect.h;
    _modified = true;
}

void TMS_Button::setW(const int w)
{
    _currentBackRect.w = w;
    _labelRect.w = w - 2 * HORIZONTAL_BORDER * w;
    _modified = true;
}

void TMS_Button::setH(const int h)
{
    _currentBackRect.h = h;
    _labelRect.h = h - 2 * VERTICAL_BORDER * h;
    _modified = true;
}

int TMS_Button::getDefaultX() const { return _defaultBackRect.x; }

int TMS_Button::getDefaultY() const { return _defaultBackRect.y; }

int TMS_Button::getDefaultW() const { return _defaultBackRect.w; }

int TMS_Button::getDefaultH() const { return _defaultBackRect.h; }

int TMS_Button::getX() const { return _currentBackRect.x; }

int TMS_Button::getY() const { return _currentBackRect.y; }

int TMS_Button::getW() const { return _currentBackRect.w; }

int TMS_Button::getH() const { return _currentBackRect.h; }

void TMS_Button::resetToDefault()
{
    setX(_defaultBackRect.x);
    setY(_defaultBackRect.y);
    setW(_defaultBackRect.w);
    setH(_defaultBackRect.h);
    _modified = true;
}

void TMS_Button::setRenderingBuffers()
{
    /* Vertex Array Object. */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    /* Vertex Buffer Object. */
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    /* Element Buffer Object. */
    glGenBuffers(1, &ebo);
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
}

bool TMS_Button::checkCollision(const int x, const int y) const
{
    if (x < _currentBackRect.x || x > _currentBackRect.x + _currentBackRect.w) return false;
    if (y < _currentBackRect.y || y > _currentBackRect.y + _currentBackRect.h) return false;
    return true;
}

bool TMS_Button::wasModified()
{
    return _modified;
}

void TMS_Button::resetModification()
{
    _modified = false;
}
