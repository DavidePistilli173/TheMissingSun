/* Temporary main file for testing. */

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "include/tms.hpp"
#include "include/tms_shader_namespace.hpp"
#include "include/tms_texture_namespace.hpp"
#include "classes/TMS_Game.hpp"
#include "classes/TMS_Shader.hpp"
#include "classes/TMS_Texture.hpp"


int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
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
    /* Enable alpha blending. */
    glEnable(GL_BLEND);
    /* Set blending factors. */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /* Set debug context. */
    #ifdef _DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(&tms::openGLError, nullptr);
    #endif

    TMS_Texture texture;
    TMS_Shader shader;
    TMS_Game game;
    game.loadGame(_windowWidth, _windowHeight);

    try
    {
        texture = TMS_Texture(tms::texture::FILES[3]);
        shader  = TMS_Shader(tms::shader::PLAIN_VERTEX, tms::shader::PLAIN_FRAGMENT);
    }
    catch (std::string e)
    {
        printf("%s", e.c_str());
        return -1;
    }

    float backgroundBox[] =
    {
        static_cast<float>(-_windowWidth),   0.0f,                               static_cast<float>(tms::Layer::BACKGROUND_LAYER), -1.0f, 0.0f, // Top left corner.
        static_cast<float>(2*_windowWidth),    0.0f,                               static_cast<float>(tms::Layer::BACKGROUND_LAYER), 1.0f, 0.0f, // Top right corner.
        static_cast<float>(2*_windowWidth),    static_cast<float>(_windowHeight),   static_cast<float>(tms::Layer::BACKGROUND_LAYER), 1.0f, 1.0f, // Bottom right corner.
        static_cast<float>(-_windowWidth),   static_cast<float>(_windowHeight),   static_cast<float>(tms::Layer::BACKGROUND_LAYER), -1.0f, 1.0f  // Bottom left corner.
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

    /* Model matrix. */
    glm::mat4 modelMat = glm::mat4(1.0f);
    /* View matrix. */
    glm::mat4 viewMat = glm::lookAt(tms::DEFAULT_CAMERA_POSITION, tms::DEFAULT_CAMERA_TARGET, tms::DEFAULT_CAMERA_UP);
    glm::vec3 currentCameraPos = tms::DEFAULT_CAMERA_POSITION;
    glm::vec3 currentCameraTarget = tms::DEFAULT_CAMERA_TARGET;
    /* Create an orthographic projection matrix. */
    glm::mat4 orthographicProjection = glm::ortho(0.0f, static_cast<float>(_windowWidth), static_cast<float>(_windowHeight), 0.0f, 0.0f, -static_cast<float>(tms::Layer::MAX_LAYER));

    /* Combine the visualisation matrices. */
    glm::mat4 mat = orthographicProjection * viewMat * modelMat;
    if (!shader.addUniform(tms::shader::plain[static_cast<int>(tms::shader::Plain::CAMERA_MATRIX)])) return -1;
    if (!shader.addUniform(tms::shader::plain[static_cast<int>(tms::shader::Plain::TEXTURE)])) return -1;
    shader.use();
    shader.setUniform(static_cast<int>(tms::shader::Plain::CAMERA_MATRIX), glm::value_ptr(mat));
    shader.setUniform(static_cast<int>(tms::shader::Plain::TEXTURE), static_cast<int>(tms::texture::Layer::LAYER_0));

    bool done = false;
    SDL_Event event;
    while (!done)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_q:
                    done = true;
                    break;
                case SDLK_w:
                    currentCameraPos.y -= 10.0f;
                    currentCameraTarget.y -= 10.0f;
                    viewMat = glm::lookAt(currentCameraPos, currentCameraTarget, tms::DEFAULT_CAMERA_UP);
                    mat = orthographicProjection * viewMat * modelMat;
                    shader.use();
                    shader.setUniform(static_cast<int>(tms::shader::Plain::CAMERA_MATRIX), glm::value_ptr(mat));
                    break;
                case SDLK_a:
                    currentCameraPos.x -= 10.0f;
                    currentCameraTarget.x -= 10.0f;
                    viewMat = glm::lookAt(currentCameraPos, currentCameraTarget, tms::DEFAULT_CAMERA_UP);
                    mat = orthographicProjection * viewMat * modelMat;
                    shader.use();
                    shader.setUniform(static_cast<int>(tms::shader::Plain::CAMERA_MATRIX), glm::value_ptr(mat));
                    break;
                case SDLK_s:
                    currentCameraPos.y += 10.0f;
                    currentCameraTarget.y += 10.0f;
                    viewMat = glm::lookAt(currentCameraPos, currentCameraTarget, tms::DEFAULT_CAMERA_UP);
                    mat = orthographicProjection * viewMat * modelMat;
                    shader.use();
                    shader.setUniform(static_cast<int>(tms::shader::Plain::CAMERA_MATRIX), glm::value_ptr(mat));
                    break;
                case SDLK_d:
                    currentCameraPos.x += 10.0f;
                    currentCameraTarget.x += 10.0f;
                    viewMat = glm::lookAt(currentCameraPos, currentCameraTarget, tms::DEFAULT_CAMERA_UP);
                    mat = orthographicProjection * viewMat * modelMat;
                    shader.use();
                    shader.setUniform(static_cast<int>(tms::shader::Plain::CAMERA_MATRIX), glm::value_ptr(mat));
                    break;
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glBindVertexArray(backgroundVAO);
        shader.use();
        texture.bind();
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        game.render(_window);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(_window.get());
    }

    return 0;
}