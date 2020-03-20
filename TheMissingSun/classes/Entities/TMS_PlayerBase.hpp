#ifndef TMS_PLAYERBASE_HPP
#define TMS_PLAYERBASE_HPP

#include "../../include/tms_shader_namespace.hpp"
#include "../../include/tms_texture_namespace.hpp"
#include "../TMS_Camera.hpp"
#include "../TMS_EventDispatcher.hpp"
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

    TMS_PlayerBase(std::vector<const TMS_Shader*>& shaders, std::vector<const TMS_Texture*>& textures,
                   const tms::Rect<float> baseRect, const TMS_ResourceContainer<TMS_Shader>& allShaders, 
                   const TMS_ResourceContainer<TMS_Texture>& allTextures, const TMS_Camera& camera, unsigned int i = 0);

    /***************** CONSTANTS *****************/
    /* Size of the building grid. */
    static constexpr int ROW_NUM = 4;
    static constexpr int COLUMN_NUM = 8;
    /* Cell that will contain the first building. */
    static constexpr int INITIAL_BUILDING_ROW = 0;
    static constexpr int INITIAL_BUILDING_COLUMN = 3;
    /* List of all required shaders. */
    static constexpr std::string_view REQUIRED_SHADERS[] = 
    {
        tms::shader::NAMES[static_cast<int>(tms::shader::Name::PLAIN)],
        tms::shader::NAMES[static_cast<int>(tms::shader::Name::HIGHLIGHT)]
    };
    /* List of all required textures. */
    static constexpr std::string_view REQUIRED_TEXTURES[] = 
    {
        tms::texture::NAMES[static_cast<int>(tms::texture::Name::TEST)]
    };

    /***************** METHODS *****************/
    /* Getters. */
    std::string_view getName() const override;
    tms::Layer getLayer() const override; // Return the entities' depth layer.
    std::vector<tms::EventType>& getRelevantEvents() override; // Return the events relevant for the current entity.
    /* Check whether (x,y) is inside the current entity or not. */
    bool checkCollision(const float x, const float y) const override;
    /* Handle each event passed to this entity. */
    void handleEvent(const SDL_Event& event) override;
    /* Render the object on screen. */
    void render() override;
    void setSpan(tms::Rect<float> span) override;

private:
    /***************** METHODS *****************/
    void _setEvents(); // Set relevant events.
    bool _setInitialState(); // Set the initial state of the base.
    bool _build(const std::string_view buildingName, const int row, const int column);

    /***************** VARIABLES *****************/
    TMS_EventDispatcher _eventDispatcher; // Event handler.
    std::unique_ptr<TMS_Building> _buildingGrid[ROW_NUM][COLUMN_NUM]; // Grid containing all possible building slots.
    tms::Rect<float> _baseRect; // Rectangle for the whole base.
    float _buildingWidth, _buildingHeight;
    TMS_ResourceContainer<TMS_Building> _buildingTypes; // Container for all existing building types.
    TMS_Building* _selectedBuilding = nullptr; // Currently selected building / building slot.
    TMS_Building* _highlightedBuilding = nullptr; // Currently highlighted building / building slot.
    const TMS_Camera& _camera; // Reference to the game camera, used to get the camera's coordinates.
};

#endif