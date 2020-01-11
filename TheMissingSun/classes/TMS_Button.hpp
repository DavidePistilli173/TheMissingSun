#ifndef TMS_BUTTON_HPP
#define TMS_BUTTON_HPP

#include <string>

class TMS_Button
{
public:
    TMS_Button();

    /**************** METHODS ****************/
    /* Setters. */
<<<<<<< Updated upstream
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
=======
    void setDefaultX(const int x);
    void setDefaultY(const int y);
    void setDefaultW(const int w);
    void setDefaultH(const int h);
    /* When moving and resizing the button, keep the label centered.*/
    void setX(const int x);
    void setY(const int y);
    void setW(const int w);
    void setH(const int h);
    void setColour(const int r, const int g, const int b, const int a);
    /* Getters. */
    int getDefaultX() const;
    int getDefaultY() const;
    int getDefaultW() const;
    int getDefaultH() const;
    int getX() const;
    int getY() const;
    int getW() const;
    int getH() const;
    SDL_Color getColour();

    /* Reset the button position to the default value. */
    void resetToDefault();

    void genRenderingBuffers(); // Generate VAO, VBO and EBO and set them.
    void setRenderingBuffers(); // Set VBO and EBO data.
    bool setLabelTexture(tms::font_t& font);

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
    SDL_Color _labelColour; // Colour information for the button's label.
    std::atomic<bool> _modified; // Set to true when the coordinates change. Set to false at the first render after the changes.
>>>>>>> Stashed changes
};

#endif