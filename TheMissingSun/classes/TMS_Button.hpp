#ifndef TMS_BUTTON_HPP
#define TMS_BUTTON_HPP

#include <string>

#include "../include/tms.hpp"

class TMS_Button
{
public:
    TMS_Button();

    /**************** METHODS ****************/
    /* Setters. */
    void setDefaultX(const int x) { _defaultBackRect.x = x; }
    void setDefaultY(const int y) { _defaultBackRect.y = y; }
    void setDefaultW(const int w) { _defaultBackRect.w = w; }
    void setDefaultH(const int h) { _defaultBackRect.h = h; }
    void setX(const int x) { _currentBackRect.x = x; }
    void setY(const int y) { _currentBackRect.y = y; }
    void setW(const int w) { _currentBackRect.w = w; }
    void setH(const int h) { _currentBackRect.h = h; }
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
        _currentBackRect.x = _defaultBackRect.x;
        _currentBackRect.y = _defaultBackRect.y;
        _currentBackRect.w = _defaultBackRect.w;
        _currentBackRect.h = _defaultBackRect.h;
    }
    /* Check whether x and y fall inside the button. */
    bool checkCollision(const int x, const int y) const;

    std::string label; // Button label.

    /* OpenGL buffers. */
    unsigned int vao, vbo, ebo;

private:
    tms::Rect _defaultBackRect; // Default button rectangle.
    tms::Rect _currentBackRect; // Current button rectangle.
};

#endif