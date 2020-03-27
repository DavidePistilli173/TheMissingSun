#include "TMS_Item.hpp"

tms::Layer TMS_Item::getLayer() const
{
    return tms::default_layer();
}

bool TMS_Item::checkCollision(const float x, const float y) const
{
    if (x >= _span.x && x <= _span.x + _span.w &&
        y >= _span.y && y <= _span.y + _span.h) return true;
    return false;
}

void TMS_Item::handleEvent(const SDL_Event& event)
{
}

void TMS_Item::render()
{
    _textures[0]->bind();
}

int TMS_Item::getAmount() const
{
    return _amount;
}

void TMS_Item::setName(const std::string& name)
{
    _name = name;
}

void TMS_Item::setTexture(const TMS_Texture* texture)
{
    if (_textures.size() == 0) _textures.push_back(texture);
    else _textures[0] = texture;
}

bool TMS_Item::setAmount(const int amount)
{
    if (amount < 0) return false;
    _amount = amount;
    return true;
}

void TMS_Item::setSpan(const tms::Rect<float> span)
{
    _span = span;
}
