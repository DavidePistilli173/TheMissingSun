#ifndef TMS_UIELEMENT_HPP
#define TMS_UIELEMENT_HPP

#include <string_view>

#include "../../../include/tms_texture_namespace.hpp"
#include "../../TMS_Sprite.hpp"
#include "../TMS_Entity.hpp"

/* Class containing a single element to be displayed in the UI. */
template <typename T>
class TMS_UIElement : public TMS_Entity
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
        TOT
    };

    TMS_UIElement(std::vector<const TMS_Shader*>& shaders, std::vector<const TMS_Texture*>& textures,
                  const T* element);

    /***************** CONSTANTS *****************/
    static constexpr float MARGIN = 0.05f; // Border width with respect to the element's size.
    static constexpr float ICON_W = 0.2f; // Width of the icon with respect to the element's size.
    static constexpr tms::Layer DEFAULT_LAYER = tms::Layer::LAYER_0; // Default rendering layer of the element.
    /* List of required shaders. */
    static constexpr std::string_view REQUIRED_SHADERS[] =
    {
        tms::shader::NAMES[static_cast<int>(tms::shader::Name::PLAIN)],
        tms::shader::NAMES[static_cast<int>(tms::shader::Name::HIGHLIGHT)]
    };
    /* List of required textures. */
    static constexpr std::string_view REQUIRED_TEXTURES[] =
    {
        tms::texture::NAMES[static_cast<int>(tms::texture::Name::UI_ELEMENT)]
    };

    /***************** METHODS *****************/
    bool checkCollision(const float x, const float y) const override;
    std::string_view getName() const override;
    tms::Layer getLayer() const override;
    std::vector<tms::EventType>& getRelevantEvents() override;
    void handleEvent(const SDL_Event& event) override;
    void nHighlight(); // Undo the element's highlighting.
    void nSelect(); // Undo the element's selection.
    void render() override;
    void setSpan(tms::Rect<float> span) override;

private:
    /***************** VARIABLES *****************/
    std::unique_ptr<TMS_Entity> _element = nullptr; // Data to be displayed.
    TMS_Texture _labelTex; // Texture for the label.
    TMS_Sprite _label, _background; // Sprites for the element's label and background.
    bool _selected = false; // True when the element is selected.
    bool _highlighted = false; // True when the mouse cursor is over the element.
};
#endif


/***************** DEFINITIONS *****************/
template <typename T>
inline bool TMS_UIElement<T>::checkCollision(const float x, const float y) const
{
    tms::Rect<float> span = _background.getSpan();
    if (x >= span.x && x <= span.x + span.w &&
        y >= span.y && y <= span.y + span.h) return true;
    return false;
}

template <typename T>
inline std::string_view TMS_UIElement<T>::getName() const
{
    return std::string_view();
}

template <typename T>
inline tms::Layer TMS_UIElement<T>::getLayer() const
{
    return DEFAULT_LAYER;
}

template <typename T>
inline std::vector<tms::EventType>& TMS_UIElement<T>::getRelevantEvents()
{
    return _relevantEvents;
}

template <typename T>
inline void TMS_UIElement<T>::handleEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT) _selected = true;
        break;
    case SDL_MOUSEMOTION:
        _highlighted = true;
        break;
    }
}

template<typename T>
inline void TMS_UIElement<T>::nHighlight()
{
    _highlighted = false;
}

template<typename T>
inline void TMS_UIElement<T>::nSelect()
{
    _selected = false;
}

template <typename T>
inline void TMS_UIElement<T>::render()
{
    if (_selected || _highlighted) _shaders[static_cast<int>(Shader::HIGHLIGHT)]->use();
    else _shaders[static_cast<int>(Shader::PLAIN)]->use();

    _background.draw();
    _label.draw();
    _element->render();
}

template <typename T>
inline void TMS_UIElement<T>::setSpan(tms::Rect<float> span)
{
    /* Set the background's span. */
    _background.setSpan(span);
    /* Set the label's span. */
    float horizontalMargin = MARGIN * span.w;
    float verticalMargin = MARGIN * span.h;
    _label.setSpan({ span.x + horizontalMargin,
                     span.y + verticalMargin,
                     (1 - ICON_W) * span.w,
                     span.h - 2 * verticalMargin });
}