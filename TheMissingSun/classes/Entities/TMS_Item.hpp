#ifndef TMS_ITEM_HPP
#define TMS_ITEM_HPP

#include "TMS_Entity.hpp"

class TMS_Item : TMS_Entity
{
public:
    TMS_Item();

    /***************** METHODS *****************/
    tms::Layer getLayer() const override; // Return the entities' depth layer.
    std::vector<tms::EventType>& getRelevantEvents() override; // Return the events relevant for the current entity.
    /* Check whether (x,y) is inside the current entity or not. */
    bool checkCollision(const float x, const float y) const override;
    /* Handle each event passed to this entity. */
    void handleEvent(const SDL_Event& event) override;
    /* Render the object on screen. */
    void render() override;

    /* Getters. */
    int getAmount() const; // Get the number of items.
    const std::string& getName() const; // Get the item's name.

    /* Setters. */
    void setName(const std::string& name);
    void setTexture(const std::shared_ptr<TMS_Texture>& texture); // Set the item's texture.
    bool setAmount(const int amount); // Change the number of items.
    void setRect(const tms::Rect<float> span); // Set the item's position.

private:
    /***************** VARIABLES *****************/
    std::string _name; // Item name.
    int _amount; // Number of items in this stack.
    tms::Rect<float> _span; // Item rectangle.
};

#endif