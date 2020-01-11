#include "../include/tms.hpp"

namespace tms
{
    /***************** CONSTEXPR FUNCTIONS *****************/
    constexpr Layer default_layer()
    {
        return static_cast<Layer>(DEFAULT_RENDER_LAYER);
    }
    constexpr int toSDLColour(float colour)
    {
        return static_cast<int>(colour * 255);
    }

    /***************** FUNCTIONS *****************/
    void stringCopy(char* destination, const char* source)
    {
        int i = 0;
        /* Copy the source string. Truncate it if it is too long. */
        while (i < (UNIFORM_NAME_LEN - 1) && source[i] != 0)
        {
            destination[i] = source[i];
            ++i;
        }
        /* Null terminate the string. */
        destination[i] = 0;
    }
}