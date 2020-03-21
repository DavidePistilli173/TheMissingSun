#include "TMS_UIScrollBar.hpp"

TMS_UIScrollBar::TMS_UIScrollBar(std::vector<const TMS_Shader*>& shaders, std::vector<const TMS_Texture*>& textures,
    tms::Rect<float> span) :
    TMS_Entity(shaders, textures),
    _background(span,
        DEFAULT_LAYER,
        textures[static_cast<int>(Texture::BACKGROUND)],
        GL_STATIC_DRAW),
    _bar({ span.x + HORIZONTAL_MARGIN,
           span.y + VERTICAL_MARGIN,
           span.w - 2 * HORIZONTAL_MARGIN,
           span.h - 2 * VERTICAL_MARGIN },
         DEFAULT_LAYER,
         textures[static_cast<int>(Texture::BAR)],
         GL_DYNAMIC_DRAW,
         TMS_Sprite::TexMode::FILL),
    _verticalRange({span.y + VERTICAL_MARGIN, span.y + VERTICAL_MARGIN})
{
    /* Check that there is the right amount of shaders and textures. */
    if (_shaders.size() != static_cast<int>(Shader::TOT)) throw std::string("Wrong number of shaders for UIScrollBar.\n");
    if (_textures.size() != static_cast<int>(Texture::TOT)) throw std::string("Wrong number of textures for UIScrollBar.\n");
    /* Set relevant events. */
    _relevantEvents.push_back(tms::EventType::MOUSE_LEFT_CLICK);
    _relevantEvents.push_back(tms::EventType::MOUSE_LEFT_CLICK_UP);
    _relevantEvents.push_back(tms::EventType::MOUSE_HOVER);
}

bool TMS_UIScrollBar::checkCollision(const float x, const float y) const
{
    tms::Rect<float> span = _background.getSpan();
    if (x >= span.x && x <= span.x + span.w &&
        y >= span.y && y <= span.y + span.h) return true;
    return false;
}

tms::Layer TMS_UIScrollBar::getLayer() const
{
    return DEFAULT_LAYER;
}

std::string_view TMS_UIScrollBar::getName() const
{
    return std::string_view();
}

std::vector<tms::EventType>& TMS_UIScrollBar::getRelevantEvents()
{
    return _relevantEvents;
}

void TMS_UIScrollBar::handleEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            tms::Rect<float> barSpan = _bar.getSpan();
            /* If the user clicked on the bar, select it and record the position of the mouse. */
            if (event.button.x >= barSpan.x && event.button.x <= barSpan.x + barSpan.w &&
                event.button.y >= barSpan.y && event.button.y <= barSpan.y + barSpan.h)
            {
                _barSelected = true;
                _lastMousePos = { event.button.x, event.button.y };
            }
        }
        break;
    case SDL_MOUSEBUTTONUP:
        /* When the user releases the left mouse button, release the scroll bar. */
        if (event.button.button == SDL_BUTTON_LEFT) _barSelected = false;
        break;
    case SDL_MOUSEMOTION:
        /* If the bar is selected, move it and update the last mouse position. */
        if (_barSelected) moveBar({ event.button.x, event.button.y });
        break;
    }
}

void TMS_UIScrollBar::moveBar(tms::Point2D<int> mouse)
{
    tms::Rect<float> barSpan = _bar.getSpan();
    float newY = barSpan.y - mouse.y + _lastMousePos.y;
    if (newY < _verticalRange.min) _bar.setPosition({ barSpan.x, _verticalRange.min });
    else if (newY > _verticalRange.max) _bar.setPosition({ barSpan.x, _verticalRange.max });
    else _bar.setPosition({ barSpan.x, newY });
    _bar.setPosition({ barSpan.x + mouse.x - _lastMousePos.x, barSpan.y });
    /* Update the last mouse position. */
    _lastMousePos = { mouse.x, mouse.y };
}

void TMS_UIScrollBar::moveBar(const float fraction)
{
    tms::Rect<float> barSpan = _bar.getSpan();
    float scrollAmount = (_verticalRange.max - _verticalRange.min) * fraction;
    float newY = barSpan.y + scrollAmount;
    if (newY < _verticalRange.min) _bar.setPosition({ barSpan.x, _verticalRange.min });
    else if (newY > _verticalRange.max) _bar.setPosition({ barSpan.x, _verticalRange.max });
    else _bar.setPosition({ barSpan.x, newY });
}

void TMS_UIScrollBar::render()
{
    _background.draw();
    _bar.draw();
}

void TMS_UIScrollBar::setBarSize(const float fraction)
{
    tms::Rect<float> span = _background.getSpan();
    _verticalRange.min = span.y + VERTICAL_MARGIN;
    float barHeight = (span.h - 2 * VERTICAL_MARGIN) * fraction;
    _verticalRange.max = _verticalRange.min + span.h - barHeight;
    _bar.setSpan({ span.x + HORIZONTAL_MARGIN,
                   _verticalRange.min,
                   span.w - 2 * HORIZONTAL_MARGIN,
                   barHeight});
}

void TMS_UIScrollBar::setSpan(tms::Rect<float> span)
{
    _background.setSpan(span);
    _verticalRange.min = span.y + VERTICAL_MARGIN;
    _verticalRange.max = _verticalRange.min;
    _bar.setSpan({ span.x + HORIZONTAL_MARGIN,
                   _verticalRange.min,
                   span.w - 2 * HORIZONTAL_MARGIN,
                   span.h - 2 * VERTICAL_MARGIN });
}
