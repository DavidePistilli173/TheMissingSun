#include "../../include/tms_building_namespace.hpp"
#include "../../include/tms_shader_namespace.hpp"
#include "../../include/tms_texture_namespace.hpp"
#include "TMS_PlayerBase.hpp"

/* Defitions of static variables. */
const std::string TMS_PlayerBase::REQUIRED_SHADERS[] =
{
    tms::shader::NAMES[static_cast<int>(tms::shader::Name::PLAIN)],
    tms::shader::NAMES[static_cast<int>(tms::shader::Name::HIGHLIGHT)]
};
const std::string TMS_PlayerBase::REQUIRED_TEXTURES[] =
{
    tms::texture::NAMES[static_cast<int>(tms::texture::Name::TEST)]
};

TMS_PlayerBase::TMS_PlayerBase(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures,
                               const tms::Rect baseRect, const TMS_ResourceContainer<TMS_Texture>& allTextures) :
    TMS_Entity(shaders, textures),
    _baseRect(baseRect)
{
    if (_shaders.size() != static_cast<int>(Shader::TOT)) throw "Wrong number of shaders for player base initialisation.\n";
    if (_textures.size() != static_cast<int>(Texture::TOT)) throw "Wrong number of textures for player base initialisation.\n";
    if (!_buildingTypes.loadResources(tms::building::BUILDING_TYPES_FILE, allTextures)) throw "Unable to load building types.\n";
    _setEvents();
}

TMS_PlayerBase::TMS_PlayerBase(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures,
                               const tms::Rect baseRect, const TMS_ResourceContainer<TMS_Texture>& allTextures,
                               unsigned int i) :
    TMS_Entity(shaders, textures, i),
    _baseRect(baseRect)
{
    if (_shaders.size() != static_cast<int>(Shader::TOT)) throw "Wrong number of shaders for player base initialisation.\n";
    if (_textures.size() != static_cast<int>(Texture::TOT)) throw "Wrong number of textures for player base initialisation.\n";
    if (!_buildingTypes.loadResources(tms::building::BUILDING_TYPES_FILE, allTextures)) throw "Unable to load building types.\n";
    _setEvents();
}

tms::Layer TMS_PlayerBase::getLayer() const
{
    return tms::default_layer();
}

std::vector<tms::EventType>& TMS_PlayerBase::getRelevantEvents()
{
    return _relevantEvents;
}

bool TMS_PlayerBase::checkCollision(const int x, const int y) const
{
    if (x >= _baseRect.x && x <= _baseRect.x + _baseRect.w &&
        y >= _baseRect.y && y <= _baseRect.y + _baseRect.h) return true;
}

std::optional<TMS_Action> TMS_PlayerBase::handleEvent(const SDL_Event& event)
{
    return std::optional<TMS_Action>();
}

void TMS_PlayerBase::render()
{
    for (int i = 0; i < ROW_NUM; ++i)
    {
        for (int j = 0; j < COLUMN_NUM; ++j)
        {
            if (_buildingGrid[i][j].has_value()) _buildingGrid[i][j].value().render();
        }
    }
}

void TMS_PlayerBase::_setEvents()
{
    _relevantEvents.push_back(tms::EventType::MOUSE_LEFT_CLICK);
    _relevantEvents.push_back(tms::EventType::MOUSE_HOVER);
}
