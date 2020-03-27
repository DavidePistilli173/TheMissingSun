#ifndef TMS_SPRITE_HPP
#define TMS_SPRITE_HPP

#include "../include/tms.hpp"
#include "TMS_Texture.hpp"
#include "TMS_Quad.hpp"

/* 2D sprite. */
class TMS_Sprite
{
public:
    /* Texture display mode. */
    enum class TexMode
    {
        FIT, // Fit the texture inside the maximum span.
        FILL // Fill the maximum span with the texture.
    };

    TMS_Sprite(const tms::Rect<float> span, const tms::Layer layer, const TMS_Texture* texture,
               GLenum usage = GL_STATIC_DRAW, TexMode texMode = TexMode::FIT);

    /***************** METHODS *****************/
    void draw() const; // Draw the sprite.
    tms::Rect<float> getSpan() const;
    void setPosition(tms::Point2D<float> pos); // Change the sprite's coordinates.
    void setSpan(const tms::Rect<float> span); // Change both position and size of the sprite.
    bool setTexture(const TMS_Texture* texture); // Change the sprite's texture.

private:
    /***************** METHODS *****************/
    void _fitTexture(); // Shrink the span to fit the current texture.

    /***************** VARIABLES *****************/
    tms::Rect<float> _span, _maxSpan; // Current and maximum shapes of the sprite.
    TexMode _texMode; // Current draw mode of the sprite.
    const TMS_Texture* _texture;
    TMS_Quad _quad;
};

#endif