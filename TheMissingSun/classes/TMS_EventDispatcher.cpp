#include "TMS_EventDispatcher.hpp"

TMS_EventDispatcher::TMS_EventDispatcher() :
    _totalEntities(0)
{
}

bool TMS_EventDispatcher::addEntity(std::shared_ptr<TMS_Entity>& entity)
{
    /* Check that the entity is in a valid layer.*/
    int index = tms::layer_index(entity->getLayer());
    if (index < 0 || index > static_cast<int>(tms::layer_num())) return false;

    /* Add the entity. */
    _layers[index].push_back(entity);

    return true;
}

bool TMS_EventDispatcher::addEntities(std::vector<std::shared_ptr<TMS_Entity>>& entities)
{
    for (auto& entity : entities)
    {
        if (!addEntity(entity)) return false;
    }
    return true;
}

void TMS_EventDispatcher::removeEntity(std::shared_ptr<TMS_Entity>& entity)
{
    /* Check that the entity is in a valid layer.*/
    int index = tms::layer_index(entity->getLayer());
    if (index >= 0 && index <= static_cast<int>(tms::layer_num()))
    {
        /* Find the correct entity. */
        std::list<std::shared_ptr<TMS_Entity>>::iterator it(_layers[index].begin());
        while (it != _layers[index].end() && *it != entity) ++it;

        /* If a match is found, remove it. */
        if (it != _layers[index].end()) _layers[index].remove(*it);
    }
}
