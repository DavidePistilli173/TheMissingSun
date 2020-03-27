#include "TMS_ConstructionMenu.hpp"

bool TMS_ConstructionMenu::checkCollision(const float x, const float y) const
{
    return _buildings.checkCollision(x, y);
}

tms::Layer TMS_ConstructionMenu::getLayer() const
{
    return DEFAULT_LAYER;
}

void TMS_ConstructionMenu::handleEvent(const SDL_Event& event)
{
    _buildings.handleEvent(event);
}

void TMS_ConstructionMenu::render()
{
    _buildings.render();
}

void TMS_ConstructionMenu::setSpan(tms::Rect<float> span)
{
    _buildings.setSpan(span);
}
