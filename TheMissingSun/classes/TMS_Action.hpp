#ifndef TMS_ACTION_HPP
#define TMS_ACTION_HPP

#include "Entities/TMS_Entity.hpp"

/* Struct containing information related to eventHandling performed by entities. */
struct TMS_Action
{
    /* All supported actions. */
    enum class ActionType
    {
        SELECT, // Select entity1.
        N_SELECT, // Deselect entity1.
        HIGHLIGHT, // Highlight entity1.
        N_HIGHLIGHT // Remove highlighting from entity1.
    };

    ActionType action; // Action to perform.
    std::shared_ptr<TMS_Entity> entity1; // Target entity.
};

#endif