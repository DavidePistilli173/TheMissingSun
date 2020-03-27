#ifndef TMS_UISCROLLBAR_HPP
#define TMS_UISCROLLBAR_HPP

#include <string_view>

#include "../../../include/tms_shader_namespace.hpp"
#include "../../../include/tms_texture_namespace.hpp"
#include "../../TMS_Sprite.hpp"
#include "../TMS_Entity.hpp"

/* Visual representation of a scroll bar. */
class TMS_UIScrollBar : public TMS_Entity
{
public:
    /***************** ENUMS *****************/
    enum class Shader
    {
        PLAIN,
        TOT
    };
    enum class Texture
    {
        BACKGROUND, 
        BAR,
        TOT
    };

    TMS_UIScrollBar(std::vector<const TMS_Shader*>& shaders, std::vector<const TMS_Texture*>& textures,
                    tms::Rect<float> span);

    /***************** CONSTANTS *****************/
    static constexpr tms::Layer DEFAULT_LAYER = tms::Layer::LAYER_0; // Default rendering layer of the element.
    static constexpr float HORIZONTAL_MARGIN = 5.0f; // Horizontal margin in px.
    static constexpr float VERTICAL_MARGIN = 5.0f; // Vertical margin in px.
    static constexpr float SCROLL_SPEED_COEFF = 0.1f; // Speed coefficient of the scroll wheel.
    /* List of required shaders. */
    static constexpr std::string_view REQUIRED_SHADERS[] =
    {
        tms::shader::NAMES[static_cast<int>(tms::shader::Name::PLAIN)]
    };
    /* List of required textures. */
    static constexpr std::string_view REQUIRED_TEXTURES[] =
    {
        tms::texture::NAMES[static_cast<int>(tms::texture::Name::UI_SCROLL_BACKGROUND)],
        tms::texture::NAMES[static_cast<int>(tms::texture::Name::UI_SCROLL_BAR)]
    };

    /***************** METHODS *****************/
    /* Check whether (x,y) is inside the current entity or not. */
    bool checkCollision(const float x, const float y) const override;
    tms::Layer getLayer() const override; // Return the entity's depth layer.
    /* Handle each event passed to this entity. */
    void handleEvent(const SDL_Event& event) override;
    void moveBar(tms::Point2D<int> mouse); // Move the bar depending on the last mouse movement.
    void moveBar(const float fraction); // Move the bar for a fraction of its movement range.
    /* Render the object on screen. */
    void render() override;
    void setBarSize(const float fraction); // Set the bar's height. 0.0f means no bar, 1.0f means full bar.
    void setSpan(tms::Rect<float> span) override; // Set the entity's boundaries.

private:
    TMS_Sprite _background, _bar; // Sprites for the background and the moving bar.
    bool _barSelected = false; // True when the user is moving the scroll bar.
    tms::Point2D<int> _lastMousePos = { 0, 0 }; // Last position of the mouse while it was pressed.
    tms::Range<float> _verticalRange = { 0.0f, 0.0f }; // Vertical scroll range.
};

#endif