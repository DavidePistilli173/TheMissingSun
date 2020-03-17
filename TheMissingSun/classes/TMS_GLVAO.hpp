#ifndef TMS_GLVAO_HPP
#define TMS_GLVAO_HPP

#include "../include/glad/glad.h"
#include "../include/tms_shader_namespace.hpp"
#include "TMS_GLBuffer.hpp"

class TMS_GLVAO
{
public:
    TMS_GLVAO(const TMS_GLBuffer<float>& vbo, const TMS_GLBuffer<unsigned int>& ebo,
              const int coordNum, const int texCoordNum);
    ~TMS_GLVAO();
    TMS_GLVAO(const TMS_GLVAO& otherVAO) = delete;
    TMS_GLVAO& operator=(const TMS_GLVAO& otherVAO) = delete;
    TMS_GLVAO(TMS_GLVAO&& otherVAO) noexcept;
    TMS_GLVAO& operator=(TMS_GLVAO&& otherVAO) noexcept;

    /***************** METHODS *****************/
    void bind();

private:
    GLuint _id;
};

#endif