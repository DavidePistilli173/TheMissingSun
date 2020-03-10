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
                               const tms::Rect baseRect, const TMS_ResourceContainer<TMS_Shader>& allShaders,
                               const TMS_ResourceContainer<TMS_Texture>& allTextures, unsigned int i) :
    TMS_Entity(shaders, textures, i),
    _baseRect(baseRect),
    _buildingWidth(baseRect.w / COLUMN_NUM),
    _buildingHeight(baseRect.h / ROW_NUM)
{
    if (_shaders.size() != static_cast<int>(Shader::TOT)) throw "Wrong number of shaders for player base initialisation.\n";
    if (_textures.size() != static_cast<int>(Texture::TOT)) throw "Wrong number of textures for player base initialisation.\n";
    if (!_buildingTypes.loadResources(tms::building::BUILDING_TYPES_FILE, allShaders, allTextures)) throw "Unable to load building types.\n";
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
            if (_buildingGrid[i][j] != nullptr) _buildingGrid[i][j]->render();
        }
    }
}

void TMS_PlayerBase::_setEvents()
{
    _relevantEvents.push_back(tms::EventType::MOUSE_LEFT_CLICK);
    _relevantEvents.push_back(tms::EventType::MOUSE_HOVER);
}

bool TMS_PlayerBase::_setInitialState()
{
    return _build(tms::building::INITIAL_BUILDING, INITIAL_BUILDING_ROW, INITIAL_BUILDING_COLUMN);
}

bool TMS_PlayerBase::_build(const std::string& buildingName, const int row, const int column)
{
    /* If this is not a buildable slot, return. */
    if (row < 0 || row >= ROW_NUM || column < 0 || column >= COLUMN_NUM ||
        _buildingGrid[row][column] == nullptr || _buildingGrid[row][column]->getName() != tms::building::EMPTY_BUILDING) return false;
    /* If there is no empty building return. */
    std::shared_ptr<TMS_Building> emptyBuilding = _buildingTypes.get(tms::building::EMPTY_BUILDING);
    if (emptyBuilding == nullptr)
    {
        printf("Missing empty building (%s).\n", tms::building::EMPTY_BUILDING.c_str());
        return false;
    }

    /* Add the building to the grid. */
    _buildingGrid[row][column] = std::make_shared<TMS_Building>(*_buildingTypes.get(buildingName));
    if (_buildingGrid[row][column] == nullptr)
    {
        printf("Could not find building %s.\n", buildingName.c_str());
        return false;
    }
    _buildingGrid[row][column]->setSpan({ column * _buildingWidth, row * _buildingHeight, _buildingWidth, _buildingHeight });
    _eventDispatcher.addEntity(_buildingGrid[row][column]);

    /* Set the nearby cells as buildable. */
    int upRow = row - 1;
    int downRow = row + 1;
    int leftColumn = column - 1;
    int rightColumn = column + 1;

    if (upRow >= 0 && _buildingGrid[upRow][column] == nullptr)
    {
        _buildingGrid[upRow][column] = std::make_shared<TMS_Building>(*emptyBuilding);
        _buildingGrid[row][column]->setSpan({ column * _buildingWidth, upRow * _buildingHeight, _buildingWidth, _buildingHeight });
        _eventDispatcher.addEntity(_buildingGrid[upRow][column]);
    }
    if (downRow < ROW_NUM && _buildingGrid[downRow][column] == nullptr)
    {
        _buildingGrid[downRow][column] = std::make_shared<TMS_Building>(*emptyBuilding);
        _buildingGrid[row][column]->setSpan({ column * _buildingWidth, downRow * _buildingHeight, _buildingWidth, _buildingHeight });
        _eventDispatcher.addEntity(_buildingGrid[downRow][column]);
    }
    if (leftColumn >= 0 && _buildingGrid[row][leftColumn] == nullptr)
    {
        _buildingGrid[row][leftColumn] = std::make_shared<TMS_Building>(*emptyBuilding);
        _buildingGrid[row][column]->setSpan({ leftColumn * _buildingWidth, row * _buildingHeight, _buildingWidth, _buildingHeight });
        _eventDispatcher.addEntity(_buildingGrid[row][leftColumn]);
    }
    if (rightColumn < COLUMN_NUM && _buildingGrid[row][rightColumn] == nullptr)
    {
        _buildingGrid[row][rightColumn] = std::make_shared<TMS_Building>(*emptyBuilding);
        _buildingGrid[row][column]->setSpan({ rightColumn * _buildingWidth, row * _buildingHeight, _buildingWidth, _buildingHeight });
        _eventDispatcher.addEntity(_buildingGrid[row][rightColumn]);
    }

    return true;
}
