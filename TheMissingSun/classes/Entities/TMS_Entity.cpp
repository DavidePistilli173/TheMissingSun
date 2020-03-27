#include "TMS_Entity.hpp"

std::string_view TMS_Entity::getName() const
{
    return _name;
}

const std::vector<tms::EventType>& TMS_Entity::getRelevantEvents()
{
    return _relevantEvents;
}

void TMS_Entity::nHighlight()
{
    _highlighted = false;
}

void TMS_Entity::nSelect()
{
    _selected = false;
}
