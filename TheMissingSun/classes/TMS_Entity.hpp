#ifndef TMS_ENTITY_HPP
#define TMS_ENTITY_HPP

#include <optional>
#include <SDL.h>

#include "TMS_Action.hpp"
#include "TMS_Shader.hpp"
#include "TMS_Texture.hpp"

/* Generic class required for processing of different objects. */
class TMS_Entity
{
public:
    /* At creation the entity must have references to all resources. */
    TMS_Entity(std::unique_ptr<std::vector<TMS_Shader>>& shaders, std::unique_ptr<std::vector<TMS_Texture>>& textures) :
        _shaders(shaders),
        _textures(textures)
    {}

    /* Getters. */
    virtual tms::Rect getLayer() = 0; // Return the entities' depth layer.
    /* Check whether (x,y) is inside the current entity or not. */
    virtual bool checkCollision(const int x, const int y) = 0;
    /* Handle each event passed to this entity. */
    virtual std::optional<TMS_Action> handleEvent(const SDL_Event& event) = 0;
    /* Render the object on screen. */
    virtual void render() = 0;

private:
    std::unique_ptr<std::vector<TMS_Shader>>& _shaders; // Vector containing all game shaders.
    std::unique_ptr<std::vector<TMS_Texture>>& _textures; // Vector containing all game textures.
};

#endif