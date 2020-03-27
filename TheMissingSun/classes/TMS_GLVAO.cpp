#include "TMS_GLVAO.hpp"

TMS_GLVAO::TMS_GLVAO(const TMS_GLBuffer<float>& vbo, const TMS_GLBuffer<unsigned int>& ebo,
                     const int coordNum, const int texCoordNum)
{
    /* Generate the VAO. */
    glGenVertexArrays(1, &_id);
    glBindVertexArray(_id);
    /* Bind the buffers. */
    vbo.bind();
    ebo.bind();
    /* Set and enable attributes. */
    int strideSize = (coordNum + texCoordNum) * sizeof(float);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS), coordNum, GL_FLOAT, GL_FALSE, strideSize, 0);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS), texCoordNum, GL_FLOAT, GL_FALSE, strideSize, reinterpret_cast<void*>(coordNum * sizeof(float)));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS));
    glBindVertexArray(0);
}

TMS_GLVAO::~TMS_GLVAO()
{
    glDeleteVertexArrays(1, &_id);
}

TMS_GLVAO::TMS_GLVAO(TMS_GLVAO&& otherVAO) noexcept
{
    _id = otherVAO._id;
    otherVAO._id = 0;
}

TMS_GLVAO& TMS_GLVAO::operator=(TMS_GLVAO&& otherVAO) noexcept
{
    if (this != &otherVAO)
    {
        glDeleteVertexArrays(1, &_id);
        _id = otherVAO._id;
        otherVAO._id = 0;
    }
    return *this;
}

void TMS_GLVAO::bind() const
{
    glBindVertexArray(_id);
}
