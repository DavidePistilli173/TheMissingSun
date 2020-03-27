#ifndef TMS_UILIST_HPP
#define TMS_UILIST_HPP

#include <algorithm>

#include "../../../include/tms_shader_namespace.hpp"
#include "../../../include/tms_texture_namespace.hpp"
#include "../../TMS_Camera.hpp"
#include "../../TMS_EventDispatcher.hpp"
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
               const tms::Rect<float> span);

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
    static constexpr tms::Layer DEFAULT_LAYER = tms::Layer::LAYER_0; // Default rendering layer.
    static constexpr float MARGIN{ 0.02f }; // Border size with respect to the size of the list.
    static constexpr float SCROLL_WIDTH{ 0.05f }; // Width of the scroll bar with respect to the width of the list.
    static constexpr unsigned int SLOT_NUM{5}; // Number of visible slots.

    /***************** METHODS *****************/
    /* Check whether (x,y) is inside the current entity or not. */
    bool checkCollision(const float x, const float y) const override;
    /* Return the entities' depth layer. */
    tms::Layer getLayer() const override;
    /* Handle each event passed to this entity. */
    void handleEvent(const SDL_Event& event) override;
    /* Render the list. */
    void render() override;
    /* Set the content to be displayed. */
    void setContents(const std::vector<const T*>& data);
    /* Set position and size of the list as well as the slots where elements will be displayed. */
    void setSpan(const tms::Rect<float> span) override;

private:
    /***************** METHODS *****************/
    void _setLayout(); // Set the list's layout.

    /***************** VARIABLES *****************/
    std::vector<tms::Rect<float>> _elementSlots; // Slots where elements can be displayed.
    std::vector<TMS_UIElement<T>> _contents; // Contents of the list.
    TMS_UIScrollBar _scrollBar; // Scroll bar for the list.
    TMS_Sprite _background; // Sprite for the background of the list.
    tms::Range<unsigned int> _visibleElements{ 0,0 }; // Indices of the currently elements to draw.
    float _scrollSpeed{ 0.0f }; // Movement speed of the scroll bar.
    TMS_EventDispatcher _eventDispatcher; // Delivers events to list elements and to the scroll bar.
    const TMS_Camera& _camera; // Used to get the camera's coordinates.
    TMS_EventData _eventData{ nullptr, nullptr }; // Data used to correctly dispatch events.
};


/***************** DEFINITIONS *****************/
template <typename T>
inline TMS_UIList<T>::TMS_UIList(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures,
                                 const tms::Rect<float> span) :
    TMS_Entity(shaders, textures),
    _background(span, DEFAULT_LAYER, textures[static_cast<int>(Texture::BACKGROUND), GL_DYNAMIC_DRAW]),
    _scrollBar({ _shaders[static_cast<int>(Shader::PLAIN)] }, 
               { _textures[static_cast<int>(Texture::SCROLL_BACKGROUND)], _textures[static_cast<int>(Texture::SCROLL_BAR)]},
               {span.x + (1 - SCROLL_WIDTH) * span.w, span.y, span.w * SCROLL_WIDTH, span.h})
{
    /* Check that there is the right amount of shaders and textures. */
    if (_shaders.size() != static_cast<int>(Shader::TOT)) throw std::string("Wrong number of shaders for UIList.\n");
    if (_textures.size() != static_cast<int>(Texture::TOT)) throw std::string("Wrong number of textures for UIList.\n");
    /* Set the relevant events for the list. */
    _relevantEvents.push_back(tms::EventType::MOUSE_HOVER);
    _relevantEvents.push_back(tms::EventType::MOUSE_LEFT_CLICK);
    _relevantEvents.push_back(tms::EventType::MOUSE_LEFT_CLICK_UP);
    _relevantEvents.push_back(tms::EventType::MOUSE_WHEEL);
    /* Set the number of slots. */
    _elementSlots.resize(SLOT_NUM, {0.0f, 0.0f, 0.0f, 0.0f});
    /* Add the scroll bar to the event dispatcher. */
    _eventDispatcher.addEntity(&_scrollBar);
    /* Set up the list's layout. */
    _setLayout();
}

template <typename T>
inline bool TMS_UIList<T>::checkCollision(const float x, const float y) const
{
    tms::Rect<float> span = _background.getSpan();
    if (x >= span.x && x <= span.x + span.w &&
        y >= span.y && y <= span.y + span.h) return true;
    return false;
}

template <typename T>
inline tms::Layer TMS_UIList<T>::getLayer() const
{
    return DEFAULT_LAYER;
}

template<typename T>
inline void TMS_UIList<T>::handleEvent(const SDL_Event& event)
{
    std::optional<TMS_Action> action = _eventDispatcher.dispatchEvent(event, _eventData, _camera.getPosition());

    if (action.has_value())
    {
        switch (action.value().action)
        {
        case TMS_Action::ActionType::HIGHLIGHT:
            if (_eventData.highlightedEntity != nullptr) _eventData.highlightedEntity->nHighlight();
            _eventData.highlightedEntity = action.value().entity1;
            break;
        case TMS_Action::ActionType::N_HIGHLIGHT:
            _eventData.highlightedEntity->nHighlight();
            _eventData.highlightedEntity = nullptr;
            break;
        case TMS_Action::ActionType::SELECT:
            if (_eventData.selectedEntity != nullptr) _eventData.selectedEntity->nSelect();
            _eventData.selectedEntity = action.value().entity1;
            break;
        case TMS_Action::ActionType::N_SELECT:
            _eventData.selectedEntity->nSelect();
            _eventData.selectedEntity = nullptr;
            break;
        }
    }
}

template<typename T>
inline void TMS_UIList<T>::render()
{
    _background.draw();
    _scrollBar.render();
    for (int i = _visibleElements.min; i < _visibleElements.max; ++i)
    {
        _contents[i].render();
    }
}

template<typename T>
inline void TMS_UIList<T>::setContents(const std::vector<const T*>& data)
{
    /* Remove the current elements from the event dispatcher.  */
    for (int i = _visibleElements.min; i <= _visibleElements.max; ++i)
    {
        _eventDispatcher.removeEntity(&_contents[i]);
    }
    _contents.clear(); // Remove the current list contents.
    /* Set up the required shaders for each UIElement. */
    std::vector<const TMS_Shader*> requiredShaders
    {
        _shaders[static_cast<int>(Shader::PLAIN)],
        _shaders[static_cast<int>(Shader::HIGHLIGHT)]
    };
    /* Set up the required textures for each UIElement. */
    std::vector<const TMS_Texture*> requiredTextures
    {
        _textures[static_cast<int>(Texture::ELEMENT)]
    };

    /* Add UIElements to the list's contents.*/
    for (int i = 0; i < data.size(); ++i)
    {
        _contents.push_back(TMS_UIElement<T>(requiredShaders, requiredTextures, data));
        if (i < _elementSlots.size())
        {
            _contents[i].setSpan(_elementSlots[i]);
            _eventDispatcher.addEntity(_contents[i]);
        }
    }
    _visibleElements = { 0, _elementSlots.size() };

    /* Set the speed and size of the scroll bar. */
    _scrollSpeed = static_cast<float>(SLOT_NUM) / _contents.size();
    _scrollBar.setBarSize(_scrollSpeed);
}

template<typename T>
inline void TMS_UIList<T>::setSpan(const tms::Rect<float> span)
{
    _background.setSpan(span);
    float barWidth{ SCROLL_WIDTH * span.w };
    _scrollBar.setSpan({ span.x + span.w - barWidth, span.y, barWidth, span.h });
    _setLayout();
}

template<typename T>
inline void TMS_UIList<T>::_setLayout()
{
    tms::Rect<float> backgroudSpan{_background.getSpan()};
    float currentMargin{ std::min(backgroudSpan.w * MARGIN, backgroudSpan.h * MARGIN) };
    float w{ backgroudSpan.w - 2 * currentMargin };
    float h{ backgroudSpan.h - 2 * currentMargin };
    float x{ backgroudSpan.x + currentMargin };
    float y{ backgroudSpan.y + currentMargin };

    for (int i{ 0 }; i < SLOT_NUM; ++i, y += backgroudSpan.h)
    {
        _elementSlots[i] = {x, y, w, h};
    }
}

#endif