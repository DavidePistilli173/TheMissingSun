#ifndef TMS_CLOCK_HPP
#define TMS_CLOCK_HPP

#include <SDL.h>

/* Clock that uses SDL_GetTicks() to measure time. */
class TMS_Clock
{
public:
    TMS_Clock() : _start(0) {};

    /**************** METHODS ****************/
    void startClock()
    {
        _start = SDL_GetTicks();
    }
    /* Get the time elapsed since the clock started. 
       After 49 days time will overflow.
    */
    int getTime() const
    {
        return SDL_GetTicks() - _start;
    };

    /**************** VARIABLES ****************/
    int _start; // Ticks at the start of the clock.
};

#endif
