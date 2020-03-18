#ifndef TMS_BUTTON_HPP
#define TMS_BUTTON_HPP

#include <string>

#include "../include/tms.hpp"
#include "TMS_Texture.hpp"

class TMS_Button
{
public:
    TMS_Button();
    ~TMS_Button();
    TMS_Button(const TMS_Button& oldButton) = delete;
    TMS_Button& operator=(const TMS_Button& oldButton) = delete;
    TMS_Button(TMS_Button&& oldButton) noexcept;
    TMS_Button& operator=(TMS_Button&& oldButton) noexcept;

    /**************** CONSTANTS ****************/
    /* Borders for the button's label. */
    static constexpr float HORIZONTAL_BORDER = 0.12f;
    static constexpr float VERTICAL_BORDER = 0.2f;
    /* Maximum length of the button's label. */
    static constexpr float MAX_TEXT_LEN = 1000;
    /* Number of bytes required for a single vertex. */
    static constexpr int STRIDE_SIZE = 5 * sizeof(float);
    /* EBO data for all buttons. */
    static constexpr unsigned int buttonVertexSequence[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    /**************** METHODS ****************/
    /* Setters. */
    void setDefaultX(const float x);
    void setDefaultY(const float y);
    void setDefaultW(const float w);
    void setDefaultH(const float h);
    /* When moving and resizing the button, keep the label centered.*/
    void setX(const float x);
    void setY(const float y);
    void setW(const float w);
    void setH(const float h);
    void setColour(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a);
    /* Getters. */
    float getDefaultX() const;
    float getDefaultY() const;
    float getDefaultW() const;
    float getDefaultH() const;
    float getX() const;
    float getY() const;
    float getW() const;
    float getH() const;
    SDL_Color getColour();

    /* Reset the button position to the default value. */
    void resetToDefault();

    void genRenderingBuffers(); // Generate VAO, VBO and EBO and set them.
    void setRenderingBuffers(); // Set VBO and EBO data.
    bool setLabelTexture(tms::font_t& font);

    /* Check whether x and y fall inside the button. */
    bool checkCollision(const float x, const float y) const;


    /**************** VARIABLES ****************/
    std::string label; // Button label.
    unsigned int vao, vbo, ebo; // OpenGL buffers for button background.
    unsigned int labelVAO, labelVBO; // OpenGL buffers for button label.
    TMS_Texture labelTexture; // Texture generated from the button's label.

private:
    tms::Rect<float> _defaultBackRect; // Default button rectangle.
    tms::Rect<float> _currentBackRect; // Current button rectangle.

    tms::Rect<float> _labelRect; // Rectangle for the button's label.
    SDL_Color _labelColour; // Colour information for the button's label.
    float _labelLenFraction; // Label texture length over max label length.
};

#endif