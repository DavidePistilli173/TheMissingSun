#include <SDL.h>

#include "../include/glad/glad.h"
#include "TMS_Shader.hpp"


TMS_Shader::TMS_Shader() :
    _id(0)
{}

TMS_Shader::TMS_Shader(const std::string vertexShader, const std::string fragmentShader)
{
    /* Open the vertex shader source code. */
    tms::file_t vertexShaderFile(
        SDL_RWFromFile(vertexShader.c_str(), "r"),
        [](SDL_RWops* file) {SDL_RWclose(file); }
    );
    if (vertexShaderFile == nullptr) throw ("Failed to open vertex shader " + vertexShader + "\n" + SDL_GetError());

    /* Open the fragment shader source code. */
    tms::file_t fragmentShaderFile(
        SDL_RWFromFile(fragmentShader.c_str(), "r"),
        [](SDL_RWops* file) {SDL_RWclose(file); }
    );
    if (fragmentShaderFile == nullptr) throw ("Failed to open fragment shader " + fragmentShader + "\n" + SDL_GetError());

    /* Strings that will contain the shader code. */
    std::string vertexShaderCode, fragmentShaderCode;
    vertexShaderCode.reserve(tms::INIT_CODE_SIZE);
    fragmentShaderCode.reserve(tms::INIT_CODE_SIZE);

    char readBuff[tms::CODE_READ_CHUNK];
    bool done = false;

    /* Read the vertex shader code. */
    while (!done)
    {
        int readCount = SDL_RWread(vertexShaderFile.get(), readBuff, sizeof(char), tms::CODE_READ_CHUNK);
        /* Exit when eof is reached. */
        if (readCount == 0)
        {
            vertexShaderCode += '0'; // Null terminate the code.
            done = true;
        }
        else
        {
            if (readCount < tms::CODE_READ_CHUNK)
            {
                readBuff[readCount] = 0; // Null terminate the code.
                done = true;
            }
            vertexShaderCode += readBuff;
        }
    }

    done = false;
    /* Read the fragment shader code. */
    while (!done)
    {
        int readCount = SDL_RWread(fragmentShaderFile.get(), readBuff, sizeof(char), tms::CODE_READ_CHUNK);
        /* Exit when eof is reached. */
        if (readCount == 0)
        {
            fragmentShaderCode += '0'; // Null terminate the code.
            done = true;
        }
        else
        {
            if (readCount < tms::CODE_READ_CHUNK)
            {
                readBuff[readCount] = 0; // Null terminate the code.
                done = true;
            }
            fragmentShaderCode += readBuff;
        }
    }

    /* Link source to shader objects. */
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    const char* vertexShaderString = vertexShaderCode.c_str();
    const char* fragmentShaderString = fragmentShaderCode.c_str();
    glShaderSource(vertexShaderId, 1, &vertexShaderString, nullptr);
    glShaderSource(fragmentShaderId, 1, &fragmentShaderString, nullptr);
    
    GLint status;
    char log[tms::SHADER_LOG_SIZE];

    /* Compile the vertex shader. */
    glCompileShader(vertexShaderId);
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glGetShaderInfoLog(vertexShaderId, tms::SHADER_LOG_SIZE, nullptr, log);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        throw ("Failed to compile vertex shader " + vertexShader + "\n" + log);
    }

    /* Compile the fragment shader. */
    glCompileShader(fragmentShaderId);
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glGetShaderInfoLog(fragmentShaderId, tms::SHADER_LOG_SIZE, nullptr, log);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        throw ("Failed to compile fragment shader " + fragmentShader + "\n" + log);
    }

    /* Attach shaders and link shader program. */
    _id = glCreateProgram();
    glAttachShader(_id, vertexShaderId);
    glAttachShader(_id, fragmentShaderId);
    glLinkProgram(_id);
    glGetProgramiv(_id, GL_LINK_STATUS, &status);
    if (status == 0)
    {
        glGetProgramInfoLog(_id, tms::SHADER_LOG_SIZE, nullptr, log);
        glDeleteProgram(_id);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        throw ("Failed to link shader from vertex shader " + vertexShader + " and fragment shader " + fragmentShader + "\n" + log);
    }

    glDetachShader(_id, vertexShaderId);
    glDetachShader(_id, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

TMS_Shader::~TMS_Shader()
{
    glDeleteProgram(_id);
}

TMS_Shader::TMS_Shader(TMS_Shader&& oldShader) noexcept
{
    this->_id = oldShader._id;
    oldShader._id = 0;
    this->_uniforms = oldShader._uniforms;
}

TMS_Shader& TMS_Shader::operator=(TMS_Shader&& oldShader) noexcept
{
    if (this != &oldShader)
    {
        glDeleteProgram(this->_id);
        this->_id = oldShader._id;
        oldShader._id = 0;
        this->_uniforms = oldShader._uniforms;
    }

    return *this;
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
    tms::stringCopy(newUniform.name, name);

    /* Look for the uniform location and save it. */
    int location = glGetUniformLocation(_id, name);
    if (location == -1)
    {
        printf("Uniform not found: %s\n", name);
        return false;
    }
    newUniform.location = static_cast<unsigned int>(location);

    /* Add the uniform to the shader. */
    _uniforms.push_back(newUniform);
    return true;
}