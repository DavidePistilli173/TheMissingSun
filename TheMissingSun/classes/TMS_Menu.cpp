#include "TMS_Menu.hpp"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL.h>

#include "../include/glad/glad.h"
#include "../include/tinyxml2.h"

TMS_Menu::TMS_Menu() :
    _currentPage(nullptr),
    _menuState(tms::GameState::MENU)
{
}

bool TMS_Menu::init(const int windowWidth, const int windowHeight)
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

void TMS_Menu::render(tms::window_t& window, const int windowWidth, const int windowHeight)
{
    /* Rendering initialisation. */
    /* Model matrix. */
    glm::mat4 modelMat = glm::mat4(1.0f);
    /* View matrix. */
    glm::mat4 viewMat = glm::mat4(1.0f);
    viewMat = glm::translate(viewMat, glm::vec3(tms::DEFAULT_VIEW_X, tms::DEFAULT_VIEW_Y, tms::DEFAULT_VIEW_Z));
    /* Create an orthographic projection matrix. */
    glm::mat4 orthographicProjection = glm::ortho(0, windowWidth, 0, windowHeight, static_cast<int>(tms::Layer::LAYER_0), static_cast<int>(tms::Layer::MAX_LAYER));

    /* Main rendering loop. */
    while (_menuState != tms::GameState::EXIT)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



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
