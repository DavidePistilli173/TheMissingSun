#include "../include/tms.hpp"

namespace tms
{
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