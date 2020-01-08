#include "TMS_Button.hpp"

TMS_Button::TMS_Button() :
    label(""),
    vao(0), vbo(0), ebo(0),
    _defaultBackRect({0,0,0,0}),
    _currentBackRect({0,0,0,0})
{
}

bool TMS_Button::checkCollision(const int x, const int y) const
{
    if (x < _currentBackRect.x || x > _currentBackRect.x + _currentBackRect.w) return false;
    if (y < _currentBackRect.y || y > _currentBackRect.y + _currentBackRect.h) return false;
    return true;
}
