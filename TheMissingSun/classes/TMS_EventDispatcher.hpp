#ifndef TMS_EVENTDISPATCHER_HPP
#define TMS_EVENTDISPATCHER_HPP

#include <set>

#include "../include/tms.hpp"
#include "./Entities/TMS_Entity.hpp"

/* Class that dispatches incoming events to the appropriate entities. */
class TMS_EventDispatcher
{
public:
    /* Comparator for entities in shared_ptr. */
    class EntityComparator
    {
    public:
        bool operator() (const std::shared_ptr<TMS_Entity>& entity1, const std::shared_ptr<TMS_Entity>& entity2) const
        {
            return *entity1 > * entity2;
        }
    };

    TMS_EventDispatcher(); // Create an empty event dispatcher.

    /***************** METHODS *****************/
    bool addEntity(std::shared_ptr<TMS_Entity>& entity); // Add a single entity to the dispatcher.
    bool addEntities(std::vector<std::shared_ptr<TMS_Entity>>& entities); // Add multiple entities to the dispatcher.

    std::optional<TMS_Action> dispatchEvent(const SDL_Event& event, std::shared_ptr<TMS_Entity>& selectedEntity);

    void removeEntity(std::shared_ptr<TMS_Entity>& entity); // Remove a specific entity from the dispatcher.

private:
    int _totalEntities; // Total number of entities in the lists.
    /* One set for each event type. Each set is ordered with respect to each entity's layer. */
    std::set<std::shared_ptr<TMS_Entity>, EntityComparator> _entities[static_cast<int>(tms::EventType::TOT)];
};

#endif
