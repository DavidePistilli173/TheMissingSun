#ifndef TMS_BUILDING_HPP
#define TMS_BUILDING_HPP

#include "../TMS_Clock.hpp"
#include "TMS_Entity.hpp"
#include "TMS_Item.hpp"

/* Redefinition of TMS_Item for readability purposes. */
using TMS_ItemProduction = TMS_Item; // Items that can/will be produced.
using TMS_ItemCost = TMS_Item; // Items that are required for production.

/* Single building inside the main base. */
class TMS_Building : public TMS_Entity
{
public:
    /* Shader list. */
    enum class Shader
    {
        PLAIN,
        HIGHLIGHT,
        TOT
    };
    /* Texture list. */
    enum class Texture
    {
        MAIN,
        TOT
    };

    TMS_Building();
    ~TMS_Building();
    TMS_Building(const TMS_Building& oldBuilding);
    TMS_Building& operator= (const TMS_Building& oldBuilding);
    TMS_Building(TMS_Building&& oldBuilding) noexcept;
    TMS_Building& operator= (TMS_Building&& oldBuilding) noexcept;


    /***************** CONSTANTS *****************/
    static const std::string REQUIRED_SHADERS[];
    static constexpr int ANIMATION_TIME = 1000; // Time to display the entire animation in ms.

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
    void setSpan(const tms::Rect& span);
    bool setShaders(const std::vector<std::shared_ptr<TMS_Shader>>& shaders);
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
    unsigned int _VAO, _VBO, _EBO; // OpenGL buffers.
    bool _highlighted; // True if the building is highlighted (eg. by the mouse cursor).
    int _currenTexture; // Index of the current texture.
    TMS_Clock _clock; // Clock used for building animations.
    int _timeStep; // Time interval between animation frames.
};

#endif