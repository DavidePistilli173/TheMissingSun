#ifndef TMS_PLAYERBASE_HPP
#define TMS_PLAYERBASE_HPP

#include "../TMS_ResourceContainer.hpp"
#include "TMS_Building.hpp"
#include "TMS_Entity.hpp"

/* Main base that the player will build. */
class TMS_PlayerBase : public TMS_Entity
{
public:
    enum class Shader
    {
        PLAIN,
        HIGHLIGHT,
        TOT
    };

    enum class Texture
    {
        TEST,
        TOT
    };

    TMS_PlayerBase(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures,
                   const tms::Rect baseRect, const TMS_ResourceContainer<TMS_Texture>& allTextures);
    TMS_PlayerBase(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures,
                   const tms::Rect baseRect, const TMS_ResourceContainer<TMS_Texture>& allTextures,
                   unsigned int i);

    /***************** CONSTANTS *****************/
    /* Size of the building grid. */
    static constexpr int ROW_NUM = 4;
    static constexpr int COLUMN_NUM = 10;
    /* Cell that will contain the first building. */
    static constexpr int INITIAL_BUILDING_ROW = 0;
    static constexpr int INITIAL_BUILDING_COLUMN = 4;
    static const std::string REQUIRED_SHADERS[]; // List of all required shaders.
    static const std::string REQUIRED_TEXTURES[]; // List of all required textures.

    /***************** METHODS *****************/
    /* Getters. */
    tms::Layer getLayer() const override; // Return the entities' depth layer.
    std::vector<tms::EventType>& getRelevantEvents() override; // Return the events relevant for the current entity.
    /* Check whether (x,y) is inside the current entity or not. */
    bool checkCollision(const int x, const int y) const override;
    /* Handle each event passed to this entity. */
    std::optional<TMS_Action> handleEvent(const SDL_Event& event) override;
    /* Render the object on screen. */
    void render() override;

private:
    /***************** METHODS *****************/
    void _setEvents(); // Set relevant events.

    /***************** VARIABLES *****************/
    std::optional<TMS_Building> _buildingGrid[ROW_NUM][COLUMN_NUM]; // Grid containing all possible building slots.
    tms::Rect _baseRect; // Rectangle for the whole base.
    TMS_ResourceContainer<TMS_Building> _buildingTypes; // Container for all existing building types.
};

#endif