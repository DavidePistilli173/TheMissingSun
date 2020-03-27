#include "TMS_Quad.hpp"

TMS_Quad::TMS_Quad(const GLenum usage) :
    _ebo(TMS_GLBuffer<unsigned int>(GL_ELEMENT_ARRAY_BUFFER, tms::EBO_QUAD_DATA, GL_STATIC_DRAW)),
    _vbo(GL_ARRAY_BUFFER, usage),
    _vao(_vbo, _ebo, VBO_COORD_COMPS, VBO_TEX_COMPS),
    _usage(usage)
{
}

TMS_Quad::TMS_Quad(const tms::Rect<float> coords, const tms::Layer layer,
                   const GLenum usage, const tms::Rect<float> texCoords) :
    _ebo(TMS_GLBuffer<unsigned int>(GL_ELEMENT_ARRAY_BUFFER, tms::EBO_QUAD_DATA, GL_STATIC_DRAW)),
    _vbo(TMS_GLBuffer<float>(GL_ARRAY_BUFFER, usage)),
    _vao(_vbo, _ebo, VBO_COORD_COMPS, VBO_TEX_COMPS),
    _usage(usage)
{
    set(coords, layer, texCoords);
}

TMS_Quad::TMS_Quad(const TMS_Quad& otherQuad) :
    _ebo(TMS_GLBuffer<unsigned int>(GL_ELEMENT_ARRAY_BUFFER, tms::EBO_QUAD_DATA, GL_STATIC_DRAW)),
    _vbo(TMS_GLBuffer<float>(GL_ARRAY_BUFFER, otherQuad._vboData, VBO_SIZE, otherQuad._usage)),
    _vao(_vbo, _ebo, VBO_COORD_COMPS, VBO_TEX_COMPS),
    _usage(otherQuad._usage)
{}

void TMS_Quad::draw() const
{
    _vao.bind();
    glDrawElements(GL_TRIANGLES, EBO_ELEMS, GL_UNSIGNED_INT, 0);
}

void TMS_Quad::move(const float x, const float y)
{
    float maxX = _vboData[5] - _vboData[0] + x;
    float maxY = _vboData[11] - _vboData[1] + y;

    /* Top left corner. */
    _vboData[0] = x;
    _vboData[1] = y;
    /* Top right corner. */
    _vboData[5] = maxX;
    _vboData[6] = y;
    /* Bottom right corner. */
    _vboData[10] = maxX;
    _vboData[11] = maxY;
    /* Bottom left corner. */
    _vboData[15] = x;
    _vboData[16] = maxY;

    /* Send the data to the buffer. */
    _vbo.setData(_vboData, VBO_SIZE);
}

void TMS_Quad::resize(const tms::Rect<float> coords)
{
    float maxX = coords.x + coords.w;
    float maxY = coords.y + coords.h;

    /* Top left corner. */
    _vboData[0] = coords.x;
    _vboData[1] = coords.y;
    /* Top right corner. */
    _vboData[5] = maxX;
    _vboData[6] = coords.y;
    /* Bottom right corner. */
    _vboData[10] = maxX;
    _vboData[11] = maxY;
    /* Bottom left corner. */
    _vboData[15] = coords.x;
    _vboData[16] = maxY;

    /* Send the data to the buffer. */
    _vbo.setData(_vboData, VBO_SIZE);
}

void TMS_Quad::set(const tms::Rect<float> coords, tms::Layer layer, const tms::Rect<float> texCoords)
{
    /* Set the VBO data. */
    float maxX = coords.x + coords.w;
    float maxY = coords.y + coords.h;
    float maxTexX = texCoords.x + texCoords.w;
    float maxTexY = texCoords.y + texCoords.h;

    /* Top left corner. */
    _vboData[0] = coords.x;
    _vboData[1] = coords.y;
    _vboData[2] = static_cast<float>(layer);
    _vboData[3] = texCoords.x;
    _vboData[4] = texCoords.y;
    /* Top right corner. */
    _vboData[5] = maxX;
    _vboData[6] = coords.y;
    _vboData[7] = static_cast<float>(layer);
    _vboData[8] = maxTexX;
    _vboData[9] = texCoords.y;
    /* Bottom right corner. */
    _vboData[10] = maxX;
    _vboData[11] = maxY;
    _vboData[12] = static_cast<float>(layer);
    _vboData[13] = maxTexX;
    _vboData[14] = maxTexY;
    /* Bottom left corner. */
    _vboData[15] = coords.x;
    _vboData[16] = maxY;
    _vboData[17] = static_cast<float>(layer);
    _vboData[18] = texCoords.x;
    _vboData[19] = maxTexY;

    /* Send the data to the buffer. */
    _vbo.setData(_vboData, VBO_SIZE);
}
