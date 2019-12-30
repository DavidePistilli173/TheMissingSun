#include "TMS_Menu.hpp"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL.h>

#include "../include/glad/glad.h"
#include "../include/tinyxml2.h"
#include "../include/tms_shader_namespace.hpp"
#include "../include/tms_texture_namespace.hpp"

TMS_Menu::TMS_Menu() :
    _currentPage(nullptr),
    _menuState(tms::GameState::MENU),
    _backgroundVAO(0),
    _backgroundVBO(0),
    _backgroundEBO(0)
{
}

bool TMS_Menu::init(const int windowWidth, const int windowHeight)
{
    /* Load menu layout. */
    if (!_loadLayout(windowWidth, windowHeight))
    {
        printf("Failed to load menu layout.\n");
        return false;
    }
    /* Load menu shaders. */
    if (!_loadShaders())
    {
        printf("Failed to load shaders.\n");
        return false;
    }
    /* Load menu textures. */
    if (!_loadTextures())
    {
        printf("Failed to load textures.\n");
        return false;
    }
    _loadVAO(windowWidth, windowHeight);

    return true;
}

void TMS_Menu::render(tms::window_t& window, const int windowWidth, const int windowHeight)
{
    /* Model matrix. */
    glm::mat4 modelMat = glm::mat4(1.0f);
    /* View matrix. */
    glm::mat4 viewMat = glm::lookAt(tms::DEFAULT_CAMERA_POSITION, tms::DEFAULT_CAMERA_TARGET, tms::DEFAULT_CAMERA_UP);
    /* Create an orthographic projection matrix. */
    glm::mat4 orthographicProjection = glm::ortho(0.0f, static_cast<float>(windowWidth), static_cast<float>(windowHeight), 0.0f, 0.0f, -static_cast<float>(tms::Layer::MAX_LAYER));
    //glm::mat4 orthographicProjection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

    /* Combine the visualisation matrices. */
    glm::mat4 visualMatrix = orthographicProjection * viewMat * modelMat;

    _shaders[static_cast<int>(Shader::PLAIN)].use();
    _shaders[static_cast<int>(Shader::PLAIN)].setUniform(static_cast<int>(tms::shader::Plain::CAMERA_MATRIX), glm::value_ptr(visualMatrix));
    _shaders[static_cast<int>(Shader::PLAIN)].setUniform(static_cast<int>(tms::shader::Plain::TEXTURE), GL_TEXTURE0);

    /* Main rendering loop. */
    while (_menuState != tms::GameState::EXIT)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _shaders[static_cast<int>(Shader::PLAIN)].use();

        /* Draw menu background. */
        _textures[static_cast<int>(Texture::BACKGROUND)].bind();
        glBindVertexArray(_backgroundVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window.get());
    }
}

tms::GameState TMS_Menu::menuLoop()
{
    /* Reset the menu to the default starting page. */
    _menuState = tms::GameState::MENU;
    _currentPage = _pages[0];

    SDL_Event event; // Contains SDL events. 
    _clock.startClock();

    while (true)
    {
        /* If the last frame updated too quickly, wait for the remaining time. */
        int updateTime = _clock.getTime();
        if (updateTime < tms::MENU_UPDATE_TIME)
        {
            SDL_Delay(tms::MENU_UPDATE_TIME - updateTime);
            updateTime = _clock.getTime(); // Update the elapsed time.
        }
        _clock.startClock();

        /* Event polling. */
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                _menuState = tms::GameState::EXIT;
                return tms::GameState::EXIT;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_q:
                    _menuState = tms::GameState::EXIT;
                    return tms::GameState::EXIT;
                    break;
                }
                break;
            }
        }
    }
}

bool TMS_Menu::_loadLayout(const int windowWidth, const int windowHeight)
{
    /* Load main menu configuration. */
    tinyxml2::XMLDocument configuration;
    configuration.LoadFile(tms::CONFIG_MM_NAME);
    if (configuration.Error())
    {
        printf("Unable to load %s\n", tms::CONFIG_MM_NAME);
        printf("%s\n", configuration.ErrorIDToName(configuration.ErrorID()));
        return false;
    }

    /* Parse the configuration and create all menu pages. */
    /* Find the root element. */
    tinyxml2::XMLElement* root = configuration.FirstChildElement(tms::CONFIG_MM_TAG_ROOT);
    if (root == nullptr)
    {
        printf("Wrong format for file %s:\n", tms::CONFIG_MM_NAME);
        return false;
    }
    /* Find the first page. */
    tinyxml2::XMLElement* pageElement = root->FirstChildElement(tms::CONFIG_MM_TAG_PAGE);
    if (pageElement == nullptr)
    {
        printf("No pages found in file %s:\n", tms::CONFIG_MM_NAME);
        return false;
    }

    /* Loop through all pages. */
    int pageId = 0;
    while (pageElement != nullptr)
    {
        /* Check whether the page ids are consecutive. */
        if (std::atoi(pageElement->FirstChildElement(tms::CONFIG_MM_TAG_ID)->FirstChild()->Value()) != pageId)
        {
            printf("Page ids are not ordered in %s.\n", tms::CONFIG_MM_NAME);
            return false;
        }

        _pages.push_back(std::shared_ptr<TMS_MenuPage>(new TMS_MenuPage)); // Add page to the menu.
        _pages[pageId]->setId(pageId); // Set the page id.
        /* Move to the next page. */
        pageElement = pageElement->NextSiblingElement();
        ++pageId;
    }

    /* Add buttons to the pages created above. */
    pageElement = root->FirstChildElement(tms::CONFIG_MM_TAG_PAGE);
    pageId = 0;
    while (pageElement != nullptr)
    {
        tinyxml2::XMLElement* buttonElement = pageElement->FirstChildElement(tms::CONFIG_MM_TAG_BUTTONS)->FirstChildElement(tms::CONFIG_MM_TAG_BUTTON);
        /* Loop through all buttons for the current page. */
        while (buttonElement != nullptr)
        {
            TMS_Button button;
            /* Get button label. */
            button.setLabel(buttonElement->FirstChildElement(tms::CONFIG_MM_TAG_LABEL)->FirstChild()->Value());
            /* Get button coordinates. */
            int x, y;
            x = static_cast<int>(std::atof(buttonElement->FirstChildElement(tms::CONFIG_MM_TAG_X)->FirstChild()->Value()) * windowWidth);
            y = static_cast<int>(std::atof(buttonElement->FirstChildElement(tms::CONFIG_MM_TAG_Y)->FirstChild()->Value()) * windowHeight);
            button.setDefaultX(x);
            button.setX(x);
            button.setDefaultY(y);
            button.setY(y);
            /* Get button dimensions. */
            int width, height;
            width = static_cast<int>(std::atof(buttonElement->FirstChildElement(tms::CONFIG_MM_TAG_WIDTH)->FirstChild()->Value()) * windowWidth);
            height = static_cast<int>(std::atof(buttonElement->FirstChildElement(tms::CONFIG_MM_TAG_HEIGHT)->FirstChild()->Value()) * windowHeight);
            button.setWidth(width);
            button.setHeight(height);
            /* Get button destination page. */
            int destinationId = std::atoi(buttonElement->FirstChildElement(tms::CONFIG_MM_TAG_DEST)->FirstChild()->Value());
            if (destinationId > static_cast<int>(_pages.size() - 1))
            {
                printf("Destination id out of range for %s.\n", tms::CONFIG_MM_NAME);
                return false;
            }
            /* Add the button to the current page. */
            if (destinationId == -1) _pages[pageId]->addButton(button, nullptr);
            else _pages[pageId]->addButton(button, _pages[destinationId]);
            /* Move to the next button. */
            buttonElement = buttonElement->NextSiblingElement();
        }
        /* Move to the next page. */
        pageElement = pageElement->NextSiblingElement();
        ++pageId;
    }

    _currentPage = _pages[0]; // Place the menu in its first page.
    return true;
}

bool TMS_Menu::_loadShaders()
{
    _shaders.reserve(static_cast<int>(Shader::TOTAL));

    /* Load plain shader. */
    try
    {
        _shaders.push_back(TMS_Shader(tms::shader::PLAIN_VERTEX, tms::shader::PLAIN_FRAGMENT));
    }
    catch (std::string error)
    {
        printf("%s", error.c_str());
        return false;
    }
    /* Load plain shader uniforms. */
    for (const auto& uniform : tms::shader::plain)
    {
        if (!_shaders[static_cast<int>(Shader::PLAIN)].addUniform(uniform))
        {
            printf("Failed to add unform to plain shader.\n");
            return false;
        }
    }

    return true;
}

bool TMS_Menu::_loadTextures()
{
    _textures.resize(static_cast<int>(Texture::TOTAL));

    /* Load background texture. */
    try
    {
        _textures[static_cast<int>(Texture::BACKGROUND)] = TMS_Texture(tms::texture::MENU_BACKGROUND);
    }
    catch (std::string error)
    {
        printf("%s", error.c_str());
        return false;
    }
    return true;
}

void TMS_Menu::_loadVAO(const int windowWidth, const int windowHeight)
{
    /* Create background buffers. */
    glGenVertexArrays(1, &_backgroundVAO);
    glBindVertexArray(_backgroundVAO);
    glGenBuffers(1, &_backgroundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _backgroundVBO);
    glGenBuffers(1, &_backgroundEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _backgroundEBO);

    /* Background vertices. */
    
    float backgroundData[] =
    {
        0.0f,                               0.0f,                               static_cast<float>(tms::Layer::BACKGROUND_LAYER), 0.0f, 0.0f, // Bottom left corner.
        static_cast<float>(windowWidth),    0.0f,                               static_cast<float>(tms::Layer::BACKGROUND_LAYER), 1.0f, 0.0f, // Bottom right corner.
        static_cast<float>(windowWidth),    static_cast<float>(windowHeight),   static_cast<float>(tms::Layer::BACKGROUND_LAYER), 1.0f, 1.0f, // Top right corner.
        0.0f,                               static_cast<float>(windowHeight),   static_cast<float>(tms::Layer::BACKGROUND_LAYER), 0.0f, 1.0f  // Top left corner.
    };
    
    /*
    float backgroundData[] =
    {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom left corner.
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom right corner.
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // Top right corner.
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f  // Top left corner.
    };
    */
    
    unsigned int backgroundIndices[] =
    {
        0, 1, 2,
        2, 3, 0
    };
    
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundData), backgroundData, GL_STATIC_DRAW);
    int strideSize = 5 * sizeof(float);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS), 3, GL_FLOAT, GL_FALSE, strideSize, 0);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS), 2, GL_FLOAT, GL_FALSE, strideSize, reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS));

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(backgroundIndices), backgroundIndices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
