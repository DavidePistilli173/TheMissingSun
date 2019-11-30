#include "TMS_Menu.hpp"

#include <SDL.h>

#include "../include/tinyxml2.h"

TMS_Menu::TMS_Menu() :
    _currentPage(nullptr)
{
}

bool TMS_Menu::init()
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
    while (pageElement != nullptr)
    {
        std::shared_ptr<TMS_MenuPage> page(new TMS_MenuPage());
        page->setId(std::atoi(pageElement->FirstChildElement(tms::CONFIG_MM_TAG_ID)->Value()));
        tinyxml2::XMLElement* buttonElement = pageElement->FirstChildElement(tms::CONFIG_MM_TAG_BUTTONS)->FirstChildElement(tms::CONFIG_MM_TAG_BUTTON);
        /* Loop through all buttons for the current page. */
        while (buttonElement != nullptr)
        {
            TMS_Button button;
            button.setLabel(buttonElement->FirstChildElement(tms::CONFIG_MM_TAG_LABEL)->FirstChild()->Value());
            printf("%s\n", button.getLabel().c_str());
        }
    }
    
    /* Create the main page. */
    _currentPage = std::shared_ptr<TMS_MenuPage>(new TMS_MenuPage());
    return true;
}

tms::GameState TMS_Menu::menuLoop()
{
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
                return tms::GameState::EXIT;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_q:
                    return tms::GameState::EXIT;
                    break;
                }
                break;
            }
        }
    }
}
