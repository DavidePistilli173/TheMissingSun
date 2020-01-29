#ifndef TMS_EVENTDISPATCHER_HPP
#define TMS_EVENTDISPATCHER_HPP

#include <list>

#include "../include/tms.hpp"
#include "TMS_Entity.hpp"

/* Class that dispatches incoming events to the appropriate entities. */
class TMS_EventDispatcher
{
public:
    TMS_EventDispatcher(); // Create an empty event dispatcher.

    /***************** METHODS *****************/
    bool addEntity(std::shared_ptr<TMS_Entity>& entity); // Add a single entity to the dispatcher.
    bool addEntities(std::vector<std::shared_ptr<TMS_Entity>>& entities); // Add multiple entities to the dispatcher.

    void removeEntity(std::shared_ptr<TMS_Entity>& entity); // Remove a specific entity from the dispatcher.

private:
    int _totalEntities; // Total number of entities in the lists.
    std::list<std::shared_ptr<TMS_Entity>> _layers[tms::layer_num()]; // List of entities sorted by layer.
};

#endif
