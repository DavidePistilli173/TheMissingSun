#ifndef TMS_CONSTRUCTIONMENU_HPP
#define TMS_CONSTRUCTIONMENU_HPP

#include "../TMS_Building.hpp"
#include "../TMS_Entity.hpp"
#include "TMS_UIList.hpp"

class TMS_ConstructionMenu : public TMS_Entity 
{
public:
    /***************** CONSTANTS *****************/
    static constexpr tms::Layer DEFAULT_LAYER{ tms::Layer::LAYER_0 };

    /***************** METHODS *****************/
    /* Check whether (x,y) is inside the current entity or not. */
    bool checkCollision(const float x, const float y) const override;
    tms::Layer getLayer() const override; // Return the entity's depth layer.
    /* Handle each event passed to this entity. */
    void handleEvent(const SDL_Event& event) override;
    /* Render the object on screen. */
    void render() override;
    void setSpan(tms::Rect<float> span) override; // Set the entity's boundaries.

private:
    TMS_UIList<TMS_Building> _buildings; // List of constructible buildings.
    const TMS_Camera& _camera; // Used to get the camera's position.
};

#endif