#ifndef TMS_BUTTON_HPP
#define TMS_BUTTON_HPP

#include <string>

class TMS_Button
{
public:
    TMS_Button();

    /**************** METHODS ****************/
    /* Setters. */
    void setDefaultX(const int x) { _defaultX = x; }
    void setDefaultY(const int y) { _defaultY = y; }
    void setX(const int x) { _x = x; }
    void setY(const int y) { _y = y; }
    void setWidth(const int width) { _width = width; }
    void setHeight(const int height) { _height = height; }
    void setLabel(const std::string label) { _label = label; };
    /* Getters. */
    int getDefaultX() const { return _defaultX; };
    int getDefaultY() const { return _defaultY; };
    int getX() const { return _x; };
    int getY() const { return _y; };
    int getWidth() const { return _width; };
    int getHeight() const { return _height; };
    std::string getLabel() const { return _label; };
    /* Reset the button position to the default value. */
    void resetPosition()
    {
        _x = _defaultX;
        _y = _defaultY;
    }

private:
    std::string _label; // Button label.
    int _defaultX, _defaultY; // Button default coordinates.
    int _x, _y; // Button coordinates.
    int _width, _height; // Button dimensions.
};

#endif