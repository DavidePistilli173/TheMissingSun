#include <cstring>

#include "../include/glad/glad.h"
#include "TMS_Shader.hpp"


TMS_Shader::TMS_Shader() :
    _id(0)
{}

TMS_Shader::~TMS_Shader()
{
    glDeleteProgram(_id);
}

bool TMS_Shader::addUniform(const char* name)
{
    Uniform newUniform;
    /* Set the uniform name. */
    if (strlen(name) > tms::UNIFORM_NAME_LEN)
    {
        printf("Uniform name too long: %s\n", name);
        return false;
    }
    strcpy(newUniform.name, name);

    /* Look for the uniform location and save it. */
    newUniform.location = glGetUniformLocation(_id, name);
    if (newUniform.location == 0)
    {
        printf("Uniform not found: %s\n", name);
        return false;
    }

    /* Add the uniform to the shader. */
    _uniforms.push_back(newUniform);
    return true;
}