#ifndef TMS_BUTTON_HPP
#define TMS_BUTTON_HPP

#include <string>

class TMS_Button
{
public:
    TMS_Button();

    /**************** METHODS ****************/
    /* Reset the button position to the default value. */
    void resetPosition()
    {
        x = defaultX;
        y = defaultY;
    }

    std::string label; // Button label.
    int defaultX, defaultY; // Button default coordinates.
    int x, y; // Button coordinates.
    int width, height; // Button dimensions.

    /* OpenGL buffers. */
    unsigned int vao, vbo, ebo;
};

#endif