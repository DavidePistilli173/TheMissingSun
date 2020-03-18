#ifndef TMS_QUAD_HPP
#define TMS_QUAD_HPP

#include "../include/tms.hpp"
#include "TMS_GLBuffer.hpp"
#include "TMS_GLVAO.hpp"

/* Vertex data for a rectangle. */
class TMS_Quad
{
public:
    TMS_Quad(const tms::Rect<float> coords, const tms::Layer layer,
             const GLenum usage = GL_STATIC_DRAW, const tms::Rect<float> texCoords = {0.0f, 0.0f, 1.0f, 1.0f});

    /***************** CONSTANTS *****************/
    static constexpr int VBO_COORD_COMPS = 3; // Number of coordinate components for each vertex.
    static constexpr int VBO_TEX_COMPS = 2; // Number of texture coordinate components for each vertex.
    static constexpr int VERTEX_NUM = 4; // Number of vertices.
    static constexpr int EBO_ELEMS = 6; // Number of elements of the EBO.
    static constexpr int VBO_SIZE = (VBO_COORD_COMPS + VBO_TEX_COMPS) * VERTEX_NUM; // Size of the VBO in elements.

    /***************** METHODS *****************/
    void draw(); // Draw the quad with the currently bound shader and texture.
    void move(const tms::Rect<float> coords); // Move the quad. 

private:
    /***************** VARIABLES *****************/
    /* OpenGL buffers. */
    TMS_GLBuffer<unsigned int> _ebo;
    TMS_GLBuffer<float> _vbo;
    TMS_GLVAO _vao;

    float _vboData[VBO_SIZE]; // Raw VBO data.

};

#endif