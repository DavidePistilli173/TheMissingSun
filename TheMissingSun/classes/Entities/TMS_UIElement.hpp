#ifndef TMS_UIELEMENT_HPP
#define TMS_UIELEMENT_HPP

#include "../TMS_Sprite.hpp"
#include "TMS_Entity.hpp"

/* Class containing a single element to be displayed in the UI. */
template <typename T>
class TMS_UIElement : public TMS_Entity
{
public:
    TMS_UIElement(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures,
                  std::shared_ptr<T> element, tms::Rect<float> span = {0.0f, 0.0f, 0.0f, 0.0f}, tms::Layer layer = tms::Layer::LAYER_0) :
        TMS_Entity(shaders, textures),
        _element(*element)
    {}

    /***************** CONSTANTS *****************/
    static constexpr float MARGIN = 0.05f; // Border width with respect to the element's size.
    static constexpr float ICON_W = 0.2f; // Width of the icon with respect to the element's size.

    /***************** METHODS *****************/


private:
    /***************** VARIABLES *****************/
    T _element; // Data to be displayed.
};


/***************** DEFINITIONS *****************/

#endif