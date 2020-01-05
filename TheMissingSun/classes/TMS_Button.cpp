#include "TMS_Button.hpp"

TMS_Button::TMS_Button() :
    label(""),
    defaultX(0), defaultY(0),
    x(0), y(0),
    width(0), height(0),
    vao(0), vbo(0), ebo(0)
{
}

bool TMS_Button::checkCollision(const int x, const int y) const
{
    if (x < this->x || x > this->x + width) return false;
    if (y < this->y || y > this->y + height) return false;
    return true;
}
