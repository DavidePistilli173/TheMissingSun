#ifndef TMS_MENU_HPP
#define TMS_MENU_HPP

#include "../include/tms.hpp"
#include "TMS_Clock.hpp"

/* Menu class. */
class TMS_Menu
{
public:
    TMS_Menu();

    /**************** METHODS ****************/
    /* Main menu loop. */
    tms::GameState menuLoop();

private:
    TMS_Clock _clock; // Input timer.
};

#endif