/* Temporary main file for testing. */

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "include/tms.hpp"
#include "include/tms_shader_namespace.hpp"
#include "include/tms_texture_namespace.hpp"
#include "classes/TMS_Shader.hpp"
#include "classes/TMS_Texture.hpp"


int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    tms::window_t _window = tms::window_t
    (
        SDL_CreateWindow(tms::GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, tms::W_DEF_WIDTH, tms::W_DEF_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP),
        [](SDL_Window* win) {SDL_DestroyWindow(win); }
    );

    int _windowWidth, _windowHeight;
    SDL_GetWindowSize(_window.get(), &_windowWidth, &_windowHeight);

    SDL_GLContext glContext = SDL_GL_CreateContext(_window.get());
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    SDL_GL_SetSwapInterval(1);
    glClearColor(tms::DEFAULT_RED, tms::DEFAULT_GREEN, tms::DEFAULT_BLUE, 1.0f);
    glViewport(0, 0, _windowWidth, _windowHeight);
    glEnable(GL_DEPTH_TEST);

    TMS_Texture texture;
    TMS_Shader shader;

    try
    {
        texture = TMS_Texture(tms::texture::MENU_BACKGROUND);
        shader  = TMS_Shader(tms::shader::PLAIN_VERTEX, tms::shader::PLAIN_FRAGMENT);
    }
    catch (std::exception e)
    {
        printf("%s", e.what());
        return -1;
    }

    float backgroundBox[] = 
    {
        -0.75f, -0.75f, 0.0f, 0.0f, 0.0f, // 0. Bottom left
        0.75f, -0.75f, 0.0f, 1.0f, 0.0f,  // 1. Bottom right
        0.75f, 0.75f, 0.0f, 1.0f, 1.0f, // 2. Top right
        -0.75f, 0.75f, 0.0f, 0.0f, 1.0f // 3. Top left
    };
    unsigned int backgroundBoxIndices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    GLuint backgroundVAO;
    glGenVertexArrays(1, &backgroundVAO);
    glBindVertexArray(backgroundVAO);

    GLuint backgroundVBO;
    glGenBuffers(1, &backgroundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundBox), backgroundBox, GL_STATIC_DRAW);

    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS));

    GLuint backgroundEBO;
    glGenBuffers(1, &backgroundEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backgroundEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(backgroundBoxIndices), backgroundBoxIndices, GL_STATIC_DRAW);

    glm::mat4 mat = glm::mat4(1.0f);
    if (!shader.addUniform(tms::shader::plain[static_cast<int>(tms::shader::Plain::CAMERA_MATRIX)])) return -1;
    if (!shader.addUniform(tms::shader::plain[static_cast<int>(tms::shader::Plain::TEXTURE)])) return -1;
    shader.use();
    shader.setUniform(static_cast<int>(tms::shader::Plain::CAMERA_MATRIX), glm::value_ptr(mat));
    shader.setUniform(static_cast<int>(tms::shader::Plain::TEXTURE), GL_TEXTURE0);

    bool done = false;
    SDL_Event event;
    while (!done)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_q)
                {
                    done = true;
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(backgroundVAO);

        texture.bind();
        shader.use();
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(_window.get());
    }

    return 0;
}