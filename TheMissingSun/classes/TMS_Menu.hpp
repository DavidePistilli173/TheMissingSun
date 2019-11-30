#ifndef TMS_MENU_HPP
#define TMS_MENU_HPP

#include <vector>

#include "../include/tms.hpp"
#include "TMS_Clock.hpp"
#include "TMS_MenuPage.hpp"

/* Menu class. */
class TMS_Menu
{
public:
    TMS_Menu();

    /**************** METHODS ****************/
    /* Initialise all menu data. */
    bool init();
    /* Main menu loop. */
    tms::GameState menuLoop();

private:
    TMS_Clock _clock; // Input timer.
    std::shared_ptr<TMS_MenuPage> _currentPage; // Current menu page.
};

#endif