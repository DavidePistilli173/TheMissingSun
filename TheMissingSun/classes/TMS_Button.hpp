#ifndef TMS_BUTTON_HPP
#define TMS_BUTTON_HPP

#include <atomic>
#include <string>

#include "../include/tms.hpp"
#include "TMS_Texture.hpp"

class TMS_Button
{
public:
    TMS_Button();
    
    TMS_Button(const TMS_Button& oldButton) = delete;
    TMS_Button& operator=(const TMS_Button& oldButton) = delete;
    TMS_Button(TMS_Button&& oldButton) noexcept;
    TMS_Button& operator=(TMS_Button&& oldButton) noexcept;

    /**************** CONSTANTS ****************/
    /* Borders for the button's label. */
    static constexpr float HORIZONTAL_BORDER = 0.1f;
    static constexpr float VERTICAL_BORDER = 0.1f;
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
    void setDefaultX(const int x);
    void setDefaultY(const int y);
    void setDefaultW(const int w);
    void setDefaultH(const int h);
    /* When moving and resizing the button, keep the label centered.*/
    void setX(const int x);
    void setY(const int y);
    void setW(const int w);
    void setH(const int h);
    /* Getters. */
    int getDefaultX() const;
    int getDefaultY() const;
    int getDefaultW() const;
    int getDefaultH() const;
    int getX() const;
    int getY() const;
    int getW() const;
    int getH() const;

    /* Reset the button position to the default value. */
    void resetToDefault();

    void genRenderingBuffers(); // Generate VAO, VBO and EBO and set them.
    void setRenderingBuffers(); // Set VBO and EBO data.

    /* Check whether x and y fall inside the button. */
    bool checkCollision(const int x, const int y) const;

    /* Check whether the button was modified since the last render. */
    bool wasModified();

    /* Reset the modification value to false. */
    void resetModification();


    /**************** VARIABLES ****************/
    std::string label; // Button label.
    unsigned int vao, vbo, ebo; // OpenGL buffers.
    TMS_Texture labelTexture; // Texture generated from the button's label.

private:
    tms::Rect _defaultBackRect; // Default button rectangle.
    tms::Rect _currentBackRect; // Current button rectangle.
    tms::Rect _labelRect; // Rectangle for the button's label.
    std::atomic<bool> _modified; // Set to true when the coordinates change. Set to false at the first render after the changes.
};

#endif