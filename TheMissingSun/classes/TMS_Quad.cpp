#include "TMS_Quad.hpp"

TMS_Quad::TMS_Quad(const tms::Rect rect, const tms::Layer layer, const GLenum usage) :
    _ebo(TMS_GLBuffer<unsigned int>(GL_ELEMENT_ARRAY_BUFFER, tms::EBO_QUAD_DATA, usage)),
    _vbo(TMS_GLBuffer<float>(GL_ARRAY_BUFFER, usage)),
    _vao(_vbo, _ebo, VBO_COORD_COMPS, VBO_TEX_COMPS)
{
    /* Set the VBO data. */
    float maxX = static_cast<float>(rect.x + rect.w);
    float maxY = static_cast<float>(rect.y + rect.h);

    /* Top left corner. */
    _vboData[0] = static_cast<float>(rect.x);
    _vboData[1] = static_cast<float>(rect.y);
    _vboData[2] = static_cast<float>(layer);
    _vboData[3] = 0.0f;
    _vboData[4] = 0.0f;
    /* Top right corner. */
    _vboData[5] = maxX;
    _vboData[6] = static_cast<float>(rect.y);
    _vboData[7] = static_cast<float>(layer);
    _vboData[8] = 1.0f;
    _vboData[9] = 0.0f;
    /* Bottom right corner. */
    _vboData[10] = maxX;
    _vboData[11] = maxY;
    _vboData[12] = static_cast<float>(layer);
    _vboData[13] = 1.0f;
    _vboData[14] = 1.0f;
    /* Bottom left corner. */
    _vboData[15] = static_cast<float>(rect.x);
    _vboData[16] = maxY;
    _vboData[17] = static_cast<float>(layer);
    _vboData[18] = 0.0f;
    _vboData[19] = 1.0f;

    /* Send the data to the buffer. */
    _vbo.setData(_vboData);
}

void TMS_Quad::draw()
{
    _vao.bind();
    glDrawElements(GL_TRIANGLES, EBO_ELEMS, GL_UNSIGNED_INT, 0);
}
