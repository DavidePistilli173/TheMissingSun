#ifndef TMS_EVENTDATA_HPP
#define TMS_EVENTDATA_HPP

#include "Entities/TMS_Entity.hpp"

/* Data needed to correctly interpret SDL events. */
struct TMS_EventData
{
    std::shared_ptr<TMS_Entity> selectedEntity = nullptr; // Currently selected entity.
    std::shared_ptr<TMS_Entity> highlightedEntity = nullptr; // Currently highlighted entity.
};

#endif