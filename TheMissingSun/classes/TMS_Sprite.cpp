#include "../include/tms.hpp"
#include "TMS_Sprite.hpp"

TMS_Sprite::TMS_Sprite(const tms::Rect<float> span, const tms::Layer layer, const TMS_Texture* texture, 
                       GLenum usage, TexMode texMode) :
    _span(span),
    _maxSpan(span),
    _texMode(texMode),
    _texture(texture),
    _quad(usage)
{
    if (_texture == nullptr) throw std::string("Sprite with no texture.\n");

    if (_texMode == TexMode::FIT) _fitTexture();
    _quad.set(_span, layer, tms::DEFAULT_TEX_COORDS);
}

void TMS_Sprite::draw()
{
    _texture->bind();
    _quad.draw();
}

tms::Rect<float> TMS_Sprite::getSpan() const
{
    return _span;
}

void TMS_Sprite::setPosition(const float x, const float y)
{
    _span.x = x;
    _maxSpan.x = x;
    _span.y = y;
    _maxSpan.y = y;
    _quad.move(x, y);
}

void TMS_Sprite::setSpan(const tms::Rect<float> span)
{
    _span = span;
    _maxSpan = span;
    if (_texMode == TexMode::FIT) _fitTexture();
    _quad.resize(_span);
}

bool TMS_Sprite::setTexture(const TMS_Texture* texture)
{
    _texture = texture;
    if (_texMode == TexMode::FIT) _fitTexture();
    return true;
}

void TMS_Sprite::_fitTexture()
{
    /* Resize the span to fit the texture without distortions. */
    float texAspectRatio = static_cast<float>(_texture->getW()) / _texture->getH();
    float spanAspectRatio = _maxSpan.w / _maxSpan.h;

    if (texAspectRatio > spanAspectRatio)
    {
        _span.h = _maxSpan.w / texAspectRatio;
        _span.y = _maxSpan.y + (_maxSpan.h - _span.h) / 2;
        _quad.resize(_span);
    }
    else if (texAspectRatio < spanAspectRatio)
    {
        _span.w = _maxSpan.h * texAspectRatio;
        _span.x = _maxSpan.x + (_maxSpan.w - _span.w) / 2;
        _quad.resize(_span);
    }
    else if (spanAspectRatio - (_span.w / _span.h) != 0)
    {
        _span = _maxSpan;
        _quad.resize(_span);
    }
}
