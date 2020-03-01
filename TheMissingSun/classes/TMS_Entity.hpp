#ifndef TMS_ENTITY_HPP
#define TMS_ENTITY_HPP

#include <optional>

#include "TMS_Action.hpp"
#include "TMS_Event.hpp"
#include "TMS_Shader.hpp"
#include "TMS_Texture.hpp"

/* Generic class required for processing of different objects. */
class TMS_Entity
{
public:
    /* At creation the entity must have references to all resources. */
    TMS_Entity(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures) :
        id(0),
        _shaders(shaders),
        _textures(textures)
    {}
    TMS_Entity(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures,
               unsigned int i) :
        id(i),
        _shaders(shaders),
        _textures(textures)
    {}

    /* Getters. */
    virtual tms::Layer getLayer() = 0; // Return the entities' depth layer.
    virtual std::vector<tms::EventType>& getRelevantEvents() = 0; // Return the events relevant for the current entity.
    /* Check whether (x,y) is inside the current entity or not. */
    virtual bool checkCollision(const int x, const int y) = 0;
    /* Handle each event passed to this entity. */
    virtual std::optional<TMS_Action> handleEvent(const SDL_Event& event) = 0;
    /* Render the object on screen. */
    virtual void render() = 0;

    unsigned int id; // ID given by the owner class.

protected:
    std::vector<std::shared_ptr<TMS_Shader>> _shaders; // Vector containing all game shaders.
    std::vector<std::shared_ptr<TMS_Texture>> _textures; // Vector containing all game textures.
    std::vector<tms::EventType> _relevantEvents; // Vector containing all relevant events for the current entity.
};

#endif