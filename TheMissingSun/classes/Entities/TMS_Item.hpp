#ifndef TMS_ITEM_HPP
#define TMS_ITEM_HPP

#include "TMS_Entity.hpp"

class TMS_Item : TMS_Entity
{
public:
    TMS_Item() = default;

    /***************** METHODS *****************/
    tms::Layer getLayer() const override; // Return the entities' depth layer.
    std::string_view getName() const override; // Get the item's name.
    std::vector<tms::EventType>& getRelevantEvents() override; // Return the events relevant for the current entity.
    /* Check whether (x,y) is inside the current entity or not. */
    bool checkCollision(const float x, const float y) const override;
    /* Handle each event passed to this entity. */
    void handleEvent(const SDL_Event& event) override;
    /* Render the object on screen. */
    void render() override;
    void setSpan(const tms::Rect<float> span) override; // Set the item's position.

    /* Getters. */
    int getAmount() const; // Get the number of items.

    /* Setters. */
    void setName(const std::string& name);
    void setTexture(const TMS_Texture* texture); // Set the item's texture.
    bool setAmount(const int amount); // Change the number of items.

private:
    /***************** VARIABLES *****************/
    std::string _name; // Item name.
    int _amount = 0; // Number of items in this stack.
    tms::Rect<float> _span = { 0.0f, 0.0f, 0.0f, 0.0f }; // Item rectangle.
};

#endif