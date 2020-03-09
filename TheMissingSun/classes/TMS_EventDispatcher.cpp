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
    std::vector<tms::EventType> relevantEvents = entity->getRelevantEvents();
    for (const auto& eventType : relevantEvents)
    {
        _entities[static_cast<int>(eventType)].insert(entity);
    }

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

std::optional<TMS_Action> TMS_EventDispatcher::dispatchEvent(const SDL_Event& event, std::shared_ptr<TMS_Entity>& selectedEntity)
{
    /* Convert the SDL event type into tms EventType and handle the event. */
    std::set<std::shared_ptr<TMS_Entity>>::iterator iterator;
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        /* Left mouse click. */
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            /* Find the correct entity and deliver the event to it. */
            iterator = _entities[static_cast<int>(tms::EventType::MOUSE_LEFT_CLICK)].begin();
            while (!(*iterator)->checkCollision(event.button.x, event.button.y)) ++iterator;
            if (iterator != _entities[static_cast<int>(tms::EventType::MOUSE_LEFT_CLICK)].end())
            {
                return (*iterator)->handleEvent(event);
            }
        }
        break;
    case SDL_MOUSEMOTION:
        /* Find the correct entity and deliver the event to it. */
        iterator = _entities[static_cast<int>(tms::EventType::MOUSE_HOVER)].begin();
        while (!(*iterator)->checkCollision(event.button.x, event.button.y)) ++iterator;
        if (iterator != _entities[static_cast<int>(tms::EventType::MOUSE_HOVER)].end())
        {
            return (*iterator)->handleEvent(event);
        }
        break;
    case SDL_KEYDOWN:
        /* Find the correct entity and deliver the event to it. */
        iterator = _entities[static_cast<int>(tms::EventType::KEY_PRESS)].find(selectedEntity);
        if (iterator != _entities[static_cast<int>(tms::EventType::KEY_PRESS)].end())
        {
            return (*iterator)->handleEvent(event);
        }
        break;
    }

    return std::optional<TMS_Action>();
}

void TMS_EventDispatcher::removeEntity(std::shared_ptr<TMS_Entity>& entity)
{
    /* Remove the entity from each set it is in. */
    std::vector<tms::EventType> relevantEvents = entity->getRelevantEvents();
    for (auto eventType : relevantEvents)
    {
        _entities[static_cast<int>(eventType)].erase(_entities[static_cast<int>(eventType)].find(entity));
    }
}
