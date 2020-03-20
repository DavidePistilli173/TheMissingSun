#include "../TMS_Building.hpp"
#include "TMS_UIElement.hpp"

template <>
TMS_UIElement<TMS_Building>::TMS_UIElement(std::vector<const TMS_Shader*>& shaders, std::vector<const TMS_Texture*>& textures, 
                             const TMS_Building* element) :
    TMS_Entity(shaders, textures),
    _element(std::make_unique<TMS_Building>(*element)),
    _labelTex(element->getName()),
    _background({0.0f, 0.0f, 0.0f, 0.0f}, DEFAULT_LAYER, textures[static_cast<int>(Texture::BACKGROUND)], GL_DYNAMIC_DRAW),
    _label({0.0f, 0.0f, 0.0f, 0.0f}, DEFAULT_LAYER, &_labelTex, GL_DYNAMIC_DRAW)
{
    if (_shaders.size() != static_cast<int>(Shader::TOT)) throw std::string("Wrong number of shaders for UIElement.\n");
    if (_textures.size() != static_cast<int>(Texture::TOT)) throw std::string("Wrong number of textures for UIElement.\n");

    _relevantEvents.push_back(tms::EventType::MOUSE_LEFT_CLICK);
    _relevantEvents.push_back(tms::EventType::MOUSE_HOVER);
}
