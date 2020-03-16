#ifndef TMS_UIELEMENT_HPP
#define TMS_UIELEMENT_HPP

#include "TMS_Entity.hpp"

/* Class containing a single element to be displayed in the UI. */
template <typename T>
class TMS_UIElement : public TMS_Entity
{
public:
    TMS_UIElement(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures,
                  std::shared_ptr<T> element) :
        TMS_Entity(shaders, textures),
        _element(*element),
        _span({ 0,0,0,0 })
    {}
    ~TMS_UIElement()
    {
        glDeleteVertexArrays(1, &_elementVAO);
        glDeleteVertexArrays(1, &_iconVAO);
        glDeleteVertexArrays(1, &_labelVAO);
        glDeleteBuffers(1, &_elementVBO);
        glDeleteBuffers(1, &_iconVBO);
        glDeleteBuffers(1, &_labelVBO);
        glDeleteBuffers(1, &_EBO);
    }

    /***************** METHODS *****************/
    void setPosition(const int x, const int y)
    {

    }
    void setSize(const int w, const int h)
    {
        _span.w = w;
        _span.h = h;
    }
    void setSpan(const tms::Rect span)
    {
        _span = span;
    }

private:
    T _element; // Data to be displayed.
    tms::Rect _span;
    /* OpenGL buffers. */
    unsigned int _EBO;
    unsigned int _elementVAO, _elementVBO; // Buffers for the whole element.
    unsigned int _iconVAO, _iconVBO; // Buffers for the icon.
    unsigned int _labelVAO, _labelVBO; // Buffers for the label.
};

#endif