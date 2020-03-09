#ifndef TMS_BUILDING_HPP
#define TMS_BUILDING_HPP

#include "TMS_Entity.hpp"
#include "TMS_Item.hpp"

/* Redefinition of TMS_Item for readability purposes. */
using TMS_ItemProduction = TMS_Item; // Items that can/will be produced.
using TMS_ItemCost = TMS_Item; // Items that are required for production.

/* Single building inside the main base. */
class TMS_Building : public TMS_Entity
{
public:
    TMS_Building(const tms::Rect span);

    /***************** METHODS *****************/
    tms::Layer getLayer() const override; // Return the entities' depth layer.
    std::vector<tms::EventType>& getRelevantEvents() override; // Return the events relevant for the current entity.
    /* Check whether (x,y) is inside the current entity or not. */
    bool checkCollision(const int x, const int y) const override;
    /* Handle each event passed to this entity. */
    std::optional<TMS_Action> handleEvent(const SDL_Event& event) override;
    /* Render the object on screen. */
    void render() override;

    /* Getters. */
    const std::string& getName() const;

    /* Setters. */
    void setName(const std::string& name);
    bool setBuildTime(const int buildTime);
    void addBuildCost(const TMS_Item& item);
    void addFixedProduction(const TMS_ItemProduction& production, const std::vector<TMS_ItemCost>& cost);
    void addContinuousProduction(const TMS_ItemProduction& production, const std::vector<TMS_ItemCost>& cost);
    void addOneTimeProduction(const TMS_ItemProduction& production, const std::vector<TMS_ItemCost>& cost);
    bool addStorage(const TMS_Item& item, const int maxCapacity);
    void addTexture(const std::shared_ptr<TMS_Texture>& texture);

private:
    /***************** VARIABLES *****************/
    std::string _name; // Name of the building.
    int _buildTime; // Build time in seconds.
    std::vector<TMS_ItemCost> _buildCost; // Items required for construction.
    std::vector<std::pair<TMS_ItemProduction, std::vector<TMS_ItemCost>>> _fixedProduction; // Fixed production of items with their fixed costs.
    std::vector<std::pair<TMS_ItemProduction, std::vector<TMS_ItemCost>>> _continuousProduction; // Continuous production of items with their costs.
    std::vector<std::pair<TMS_ItemProduction, std::vector<TMS_ItemCost>>> _oneTimeProduction; // One-Time production of items with their costs.
    std::vector<std::pair<TMS_Item, int>> _storage; // Item storage with maximum capacity.
    tms::Rect _span; // Building rectangle.
};

#endif