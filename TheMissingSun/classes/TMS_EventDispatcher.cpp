#include "TMS_EventDispatcher.hpp"

TMS_EventDispatcher::TMS_EventDispatcher() :
    _totalEntities(0)
{
}

bool TMS_EventDispatcher::addEntity(const std::shared_ptr<TMS_Entity>& entity)
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

bool TMS_EventDispatcher::addEntities(const std::vector<std::shared_ptr<TMS_Entity>>& entities)
{
    for (auto& entity : entities)
    {
        if (!addEntity(entity)) return false;
    }
    return true;
}

std::optional<TMS_Action> TMS_EventDispatcher::dispatchEvent(const SDL_Event& event, const TMS_EventData& eventData,
                                                             const glm::fvec2 cameraPos)
{
    /* Convert the SDL event type into tms EventType and handle the event. */
    std::set<std::shared_ptr<TMS_Entity>>::iterator iterator;
    std::optional<TMS_Action> action = std::optional<TMS_Action>(TMS_Action()); // Return value;
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        /* Left mouse click. */
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            /* Find the correct entity and deliver the event to it. */
            iterator = _entities[static_cast<int>(tms::EventType::MOUSE_LEFT_CLICK)].begin();

            while (iterator != _entities[static_cast<int>(tms::EventType::MOUSE_LEFT_CLICK)].end() &&
                   !(*iterator)->checkCollision(event.button.x + cameraPos.x, event.button.y + cameraPos.y)) ++iterator;

            if (iterator != _entities[static_cast<int>(tms::EventType::MOUSE_LEFT_CLICK)].end())
            {
                (*iterator)->handleEvent(event);
                action.value().action = TMS_Action::ActionType::SELECT;
                action.value().entity1 = *iterator;
            }
            else
            {
                action.value().action = TMS_Action::ActionType::N_SELECT;
                action.value().entity1 = eventData.selectedEntity;
            }
            return action;
        }
        break;
    case SDL_MOUSEMOTION:
        /* Find the correct entity and deliver the event to it. */
        iterator = _entities[static_cast<int>(tms::EventType::MOUSE_HOVER)].begin();

        while (iterator != _entities[static_cast<int>(tms::EventType::MOUSE_HOVER)].end() &&
               !(*iterator)->checkCollision(event.button.x + cameraPos.x, event.button.y + cameraPos.y)) ++iterator;

        if (iterator != _entities[static_cast<int>(tms::EventType::MOUSE_HOVER)].end())
        {
            (*iterator)->handleEvent(event);
            action.value().action = TMS_Action::ActionType::HIGHLIGHT;
            action.value().entity1 = *iterator;
        }
        else
        {
            action.value().action = TMS_Action::ActionType::N_HIGHLIGHT;
            action.value().entity1 = eventData.highlightedEntity;
        }
        return action;
        break;
    case SDL_KEYDOWN:
        /* Find the correct entity and deliver the event to it. */
        if (eventData.selectedEntity != nullptr)
        {
            iterator = _entities[static_cast<int>(tms::EventType::KEY_PRESS)].find(eventData.selectedEntity);
            if (iterator != _entities[static_cast<int>(tms::EventType::KEY_PRESS)].end())
            {
                (*iterator)->handleEvent(event);
                return std::optional<TMS_Action>();
            }
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
