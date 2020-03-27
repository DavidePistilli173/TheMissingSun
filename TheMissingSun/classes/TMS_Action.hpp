#ifndef TMS_ACTION_HPP
#define TMS_ACTION_HPP

#include "Entities/TMS_Entity.hpp"

/* Struct containing information related to eventHandling performed by entities. */
struct TMS_Action
{
    /* All supported actions. */
    enum class ActionType
    {
        NO_ACTION, // Default value.
        SELECT, // Select entity1.
        N_SELECT, // Deselect entity1.
        HIGHLIGHT, // Highlight entity1.
        N_HIGHLIGHT // Remove highlighting from entity1.
    };

    ActionType action{ ActionType::NO_ACTION }; // Action to perform.
    TMS_Entity* entity1{ nullptr }; // Target entity.
};

#endif