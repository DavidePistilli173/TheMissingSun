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
    ~TMS_Shader(); // Correctly destroy the shader.

    /**************** METHODS ****************/
    /* Use the shader program. */
    void use()
    {
        glUseProgram(_id);
    }
    /* Add a uniform to the shader. */
    bool addUniform(const char* name);

private:
    unsigned int _id; // Shader ID.
    std::vector<Uniform> _uniforms; // List of all uniforms for the current shader.
};

#endif