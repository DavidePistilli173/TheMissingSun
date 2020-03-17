#ifndef TMS_QUAD_HPP
#define TMS_QUAD_HPP

#include "../include/tms.hpp"
#include "TMS_GLBuffer.hpp"
#include "TMS_GLVAO.hpp"

/* Vertex data for a rectangle. */
class TMS_Quad
{
public:
    TMS_Quad(const tms::Rect rect, const tms::Layer layer, const GLenum usage = GL_STATIC_DRAW);

    /***************** CONSTANTS *****************/
    static constexpr int VBO_COORD_COMPS = 3; // Number of coordinate components for each vertex.
    static constexpr int VBO_TEX_COMPS = 2; // Number of texture coordinate components for each vertex.
    static constexpr int VERTEX_NUM = 4; // Number of vertices.
    static constexpr int EBO_ELEMS = 6; // Number of elements of the EBO.
    static constexpr int VBO_SIZE = (VBO_COORD_COMPS + VBO_TEX_COMPS) * VERTEX_NUM; // Size of the VBO in elements.

    /***************** METHODS *****************/
    void draw();

private:
    TMS_GLBuffer<unsigned int> _ebo;
    float _vboData[VBO_SIZE]; // Raw VBO data.
    TMS_GLVAO _vao;
    TMS_GLBuffer<float> _vbo;
};

#endif