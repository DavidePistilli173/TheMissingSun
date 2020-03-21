#ifndef TMS_UILIST_HPP
#define TMS_UILIST_HPP

#include "../../../include/tms_shader_namespace.hpp"
#include "../../../include/tms_texture_namespace.hpp"
#include "../../TMS_Camera.hpp"
#include "../TMS_Entity.hpp"
#include "TMS_UIElement.hpp"
#include "TMS_UIScrollBar.hpp"

template <typename T>
class TMS_UIList : public TMS_Entity
{
public:
    /***************** ENUMS *****************/
    enum class Shader
    {
        PLAIN,
        HIGHLIGHT,
        TOT
    };
    enum class Texture
    {
        BACKGROUND,
        ELEMENT,
        SCROLL_BACKGROUND,
        SCROLL_BAR,
        TOT
    };

    TMS_UIList(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures,
               const TMS_Camera& camera) :
        TMS_Entity(shaders, textures)
    {
        /* Check that there is the right amount of shaders and textures. */
        if (_shaders.size() != static_cast<int>(Shader::TOT)) throw std::string("Wrong number of shaders for UIList.\n");
        if (_textures.size() != static_cast<int>(Texture::TOT)) throw std::string("Wrong number of textures for UIList.\n");
        /* Set the relevant events for the list. */
        _relevantEvents.push_back(tms::EventType::MOUSE_HOVER);
        _relevantEvents.push_back(tms::EventType::MOUSE_LEFT_CLICK);
        _relevantEvents.push_back(tms::EventType::MOUSE_LEFT_CLICK_UP);
        _relevantEvents.push_back(tms::EventType::MOUSE_WHEEL);
    }

    /***************** CONSTANTS *****************/
    /* List of all required shaders. */
    static constexpr std::string_view REQUIRED_SHADERS[] = 
    {
        tms::shader::NAMES[static_cast<int>(tms::shader::Name::PLAIN)],
        tms::shader::NAMES[static_cast<int>(tms::shader::Name::HIGHLIGHT)]
    };
    /* List of all required textures. */
    static constexpr std::string_view REQUIRED_TEXTURES[] = 
    {
        tms::texture::NAMES[static_cast<int>(tms::texture::Name::UI_LIST_BACKGROUND)],
        tms::texture::NAMES[static_cast<int>(tms::texture::Name::UI_ELEMENT)],
        tms::texture::NAMES[static_cast<int>(tms::texture::Name::UI_SCROLL_BACKGROUND)],
        tms::texture::NAMES[static_cast<int>(tms::texture::Name::UI_SCROLL_BAR)]
    };

    /***************** METHODS *****************/
    /* Check whether (x,y) is inside the current entity or not. */
    bool checkCollision(const float x, const float y)
    {
        if (x >= _span.x && x <= _span.x + _span.w &&
            y >= _span.y && y <= _span.y + _span.h) return true;
        return false;
    }
    /* Return the entities' depth layer. */
    tms::Layer getLayer() const
    {
        tms::Layer::LAYER_0;
    }
    /* Handle each event passed to this entity. */
    void handleEvent(const SDL_Event& event)
    {

    }
    /* Render the list. */
    void render() override
    {

    }
    /* Set the content to be displayed. */
    void setContents(const std::vector<std::shared_ptr<T>>& data)
    {
        /* Copy the input data. */

    }
    /* Set position and size of the list. */
    void setSpan(const tms::Rect<float> span)
    {
        _span = span;
    }

private:
    std::vector<TMS_UIElement<T>> _contents; // Contents of the list.
    TMS_UIScrollBar _scrollBar; // Scroll bar for the list.
    tms::Rect<float> _span = {0.0f, 0.0f, 0.0f, 0.0f}; // Position and size of the list.
};

#endif