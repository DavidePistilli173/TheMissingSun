#include "TMS_Background.hpp"

/* Build a two-level background: first level has the sky/surface, the second one has the underground. */
TMS_Background::TMS_Background(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures,
                               const tms::Rect<float> span, const int winW, const int winH, const unsigned int i) :
    TMS_Entity(shaders, textures, i),
    _skyQuad({ span.x, span.y, span.w, span.h * LEVEL_HEIGHT_FRACTION }, 
             this->getLayer(), 
             GL_STATIC_DRAW,
             { 0.0f, 0.0f, span.w/winW, span.h * LEVEL_HEIGHT_FRACTION / winH }),
    _surfaceQuad({ span.x, span.y, span.w, span.h * LEVEL_HEIGHT_FRACTION }, 
                 this->getLayer(), 
                 GL_STATIC_DRAW,
                 { 0.0f, 0.0f, span.w / winW, span.h * LEVEL_HEIGHT_FRACTION / winH }),
    _undergroundQuad({ span.x, span.y + span.h * LEVEL_HEIGHT_FRACTION, span.w, span.h * (1 - LEVEL_HEIGHT_FRACTION) }, 
                     this->getLayer(), 
                     GL_STATIC_DRAW,
                     { 0.0f, 0.0f, span.w / winW, span.h * (1 - LEVEL_HEIGHT_FRACTION) / winH }),
    _winW(winW), _winH(winH)
{
    if (_shaders.size() != static_cast<int>(Shader::TOT)) throw "Wrong number of shaders for background initialisation.\n";
    if (_textures.size() != static_cast<int>(Texture::TOT)) throw "Wrong number of textures for background initialisation.\n";

    _setEvents();
}

tms::Layer TMS_Background::getLayer() const
{
    return tms::Layer::BACKGROUND_LAYER;
}

bool TMS_Background::checkCollision(const float x, const float y) const
{
    return true;
}

std::vector<tms::EventType>& TMS_Background::getRelevantEvents()
{
    return _relevantEvents;
}

void TMS_Background::handleEvent(const SDL_Event& event)
{
}

void TMS_Background::render()
{
    _shaders[static_cast<int>(Shader::PLAIN)]->use();

    /* Draw the sky. */
    _textures[static_cast<int>(Texture::SKY)]->bind();
    _skyQuad.draw();

    /* Draw the surface. */
    _textures[static_cast<int>(Texture::SURFACE)]->bind();
    _surfaceQuad.draw();

    /* Draw the underground. */
    _textures[static_cast<int>(Texture::UNDERGROUND)]->bind();
    _undergroundQuad.draw();

    glBindVertexArray(0);
}

void TMS_Background::_setEvents()
{
    _relevantEvents.push_back(tms::EventType::MOUSE_LEFT_CLICK);
}
