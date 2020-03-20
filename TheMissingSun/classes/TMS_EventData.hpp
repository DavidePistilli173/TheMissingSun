#ifndef TMS_EVENTDATA_HPP
#define TMS_EVENTDATA_HPP

#include "Entities/TMS_Entity.hpp"

/* Data needed to correctly interpret SDL events. */
struct TMS_EventData
{
    TMS_Entity* selectedEntity = nullptr; // Currently selected entity.
    TMS_Entity* highlightedEntity = nullptr; // Currently highlighted entity.
};

#endif