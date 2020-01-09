#ifndef TMS_BUTTON_HPP
#define TMS_BUTTON_HPP

#include <string>

#include "../include/tms.hpp"
#include "TMS_Texture.hpp"

class TMS_Button
{
public:
    TMS_Button();

    /**************** CONSTANTS ****************/
    /* Borders for the button's label. */
    static constexpr float HORIZONTAL_BORDER = 0.1f;
    static constexpr float VERTICAL_BORDER = 0.1f;

    /**************** METHODS ****************/
    /* Setters. */
    void setDefaultX(const int x) { _defaultBackRect.x = x; }
    void setDefaultY(const int y) { _defaultBackRect.y = y; }
    void setDefaultW(const int w) { _defaultBackRect.w = w; }
    void setDefaultH(const int h) { _defaultBackRect.h = h; }
    /* When moving and resizing the button, keep the label centered.*/
    void setX(const int x) 
    { 
        _currentBackRect.x = x;
        _labelRect.x = x + HORIZONTAL_BORDER * _currentBackRect.w;
    }
    void setY(const int y) 
    { 
        _currentBackRect.y = y;
        _labelRect.y = y + VERTICAL_BORDER * _currentBackRect.h;
    }
    void setW(const int w) 
    { 
        _currentBackRect.w = w;
        _labelRect.w = w - 2 * HORIZONTAL_BORDER * w;
    }
    void setH(const int h) 
    { 
        _currentBackRect.h = h; 
        _labelRect.h = h - 2 * VERTICAL_BORDER * h;
    }
    /* Getters. */
    int getDefaultX() const { return _defaultBackRect.x; }
    int getDefaultY() const { return _defaultBackRect.y; }
    int getDefaultW() const { return _defaultBackRect.w; }
    int getDefaultH() const { return _defaultBackRect.h; }
    int getX() const { return _currentBackRect.x; }
    int getY() const { return _currentBackRect.y; }
    int getW() const { return _currentBackRect.w; }
    int getH() const { return _currentBackRect.h; }

    /* Reset the button position to the default value. */
    void resetToDefault()
    {
        setX(_defaultBackRect.x);
        setY(_defaultBackRect.y);
        setW(_defaultBackRect.w);
        setH(_defaultBackRect.h);
    }

    /* Check whether x and y fall inside the button. */
    bool checkCollision(const int x, const int y) const;

    std::string label; // Button label.

    /* OpenGL buffers. */
    unsigned int vao, vbo, ebo;

    TMS_Texture labelTexture; // Texture generated from the button's label.

private:
    tms::Rect _defaultBackRect; // Default button rectangle.
    tms::Rect _currentBackRect; // Current button rectangle.
    tms::Rect _labelRect; // Rectangle for the button's label.
};

#endif