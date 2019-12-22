#ifndef TMS_SHADER_HPP
#define TMS_SHADER_HPP

#include <vector>

#include "../include/tms.hpp"

class TMS_Shader
{
public:
    /* Store uniform data. */
    struct Uniform
    {
        char name[tms::UNIFORM_NAME_LEN]; // Uniform name.
        unsigned int location; // Uniform OpenGL location.
    };

    TMS_Shader(); // Build an empty shader.
    /* Build a shader from source files. Throws std::string in case of error. */
    TMS_Shader(const std::string vertexShader, const std::string fragmentShader); 
    ~TMS_Shader(); // Correctly destroy the shader.
    TMS_Shader(TMS_Shader&& oldShader) noexcept; // Move constructor.
    TMS_Shader& operator=(TMS_Shader&& oldShader) noexcept; // Move assignment operator.

    /**************** METHODS ****************/
    /* Use the shader program. */
    void use()
    {
        glUseProgram(_id);
    }
    /* Add a uniform to the shader. */
    bool addUniform(const char* name);

    /* Uniform setters. */
    /* Set a Mat4 uniform. */
    void setUniform(const int uniformId, const GLfloat* matrix)
    {
        glUniformMatrix4fv(_uniforms[uniformId].location, 1, GL_FALSE, matrix);
    }

private:
    unsigned int _id; // Shader ID.
    std::vector<Uniform> _uniforms; // List of all uniforms for the current shader.

    /* Shader class cannot be copied. */
    TMS_Shader(const TMS_Shader& oldShader);
    TMS_Shader& operator = (const TMS_Shader& oldTexture);
};

#endif