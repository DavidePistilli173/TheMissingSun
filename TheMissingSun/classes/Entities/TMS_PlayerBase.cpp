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
                               const tms::Rect<float> baseRect, const TMS_ResourceContainer<TMS_Shader>& allShaders,
                               const TMS_ResourceContainer<TMS_Texture>& allTextures, const TMS_Camera& camera, unsigned int i) :
    TMS_Entity(shaders, textures, i),
    _baseRect(baseRect),
    _buildingWidth(baseRect.w / COLUMN_NUM),
    _buildingHeight(baseRect.h / ROW_NUM),
    _camera(camera)
{
    if (_shaders.size() != static_cast<int>(Shader::TOT)) throw std::string("Wrong number of shaders for player base initialisation.\n");
    if (_textures.size() != static_cast<int>(Texture::TOT)) throw std::string("Wrong number of textures for player base initialisation.\n");
    if (!_buildingTypes.loadResources(tms::building::BUILDING_TYPES_FILE, allShaders, allTextures)) throw std::string("Unable to load building types.\n");
    if (!_setInitialState()) throw std::string("Unable to initialise the player's base.\n");
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

bool TMS_PlayerBase::checkCollision(const float x, const float y) const
{
    if (x >= _baseRect.x && x <= _baseRect.x + _baseRect.w &&
        y >= _baseRect.y && y <= _baseRect.y + _baseRect.h) return true;
    return false;
}

void TMS_PlayerBase::handleEvent(const SDL_Event& event)
{
    /* Deliver the event to the relevant entity. */
    TMS_EventData eventData;
    eventData.selectedEntity = _selectedBuilding;
    eventData.highlightedEntity = _highlightedBuilding;
    std::optional<TMS_Action> action = _eventDispatcher.dispatchEvent(event, eventData, _camera.getPosition());

    /* Perform any required action. */
    if (action.has_value())
    {
        switch (action.value().action)
        {
        /* Select a building. */
        case TMS_Action::ActionType::SELECT:
            if (_selectedBuilding != nullptr)
            {
                if (_selectedBuilding != action.value().entity1) _selectedBuilding->nSelect();
            }
            _selectedBuilding = std::static_pointer_cast<TMS_Building>(action.value().entity1);
            break;
        /* Deselect the currently selected building. */
        case TMS_Action::ActionType::N_SELECT:
            if (_selectedBuilding != nullptr) _selectedBuilding->nSelect();
            _selectedBuilding = nullptr;
            break;
        /* Highlight a building. */
        case TMS_Action::ActionType::HIGHLIGHT:
            if (_highlightedBuilding != nullptr)
            {
                if (_highlightedBuilding != action.value().entity1) _highlightedBuilding->nHighlight();
            }
            _highlightedBuilding = std::static_pointer_cast<TMS_Building>(action.value().entity1);
            break;
        /* Remove highlighting from the currently highlighted building. */
        case TMS_Action::ActionType::N_HIGHLIGHT:
            if (_highlightedBuilding != nullptr) _highlightedBuilding->nHighlight();
            _highlightedBuilding = nullptr;
            break;
        }
    }
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
    /* Set the first buildable spot. */
    std::shared_ptr<TMS_Building> emptyBuilding = _buildingTypes.get(tms::building::EMPTY_BUILDING);
    if (emptyBuilding == nullptr)
    {
        printf("Missing emtpy building (%s).\n", tms::building::EMPTY_BUILDING.c_str());
        return false;
    }
    _buildingGrid[INITIAL_BUILDING_ROW][INITIAL_BUILDING_COLUMN] = std::make_shared<TMS_Building>(*emptyBuilding);
    _buildingGrid[INITIAL_BUILDING_ROW][INITIAL_BUILDING_COLUMN]->setSpan({ _baseRect.x + INITIAL_BUILDING_COLUMN * _buildingWidth, 
                                                                            _baseRect.y + INITIAL_BUILDING_ROW * _buildingHeight, 
                                                                            _buildingWidth, 
                                                                            _buildingHeight });
    if (!_build(tms::building::INITIAL_BUILDING, INITIAL_BUILDING_ROW, INITIAL_BUILDING_COLUMN))
    {
        printf("Unable to create the first building of the base.\n");
        return false;
    }
    return true;
}

bool TMS_PlayerBase::_build(const std::string& buildingName, const int row, const int column)
{
    /* If this is not a buildable slot, return. */
    if (row < 0 || row >= ROW_NUM || column < 0 || column >= COLUMN_NUM ||
        _buildingGrid[row][column] == nullptr || _buildingGrid[row][column]->getName() != tms::building::EMPTY_BUILDING)
    {
        printf("This is not a buildable spot ([%d, %d]).\n", row, column);
        return false;
    }

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
    _buildingGrid[row][column]->setSpan({ _baseRect.x + column * _buildingWidth, 
                                          _baseRect.y + row * _buildingHeight, 
                                          _buildingWidth, 
                                          _buildingHeight });
    _eventDispatcher.addEntity(_buildingGrid[row][column]);

    /* Set the nearby cells as buildable. */
    int upRow = row - 1;
    int downRow = row + 1;
    int leftColumn = column - 1;
    int rightColumn = column + 1;

    if (upRow >= 0 && _buildingGrid[upRow][column] == nullptr)
    {
        _buildingGrid[upRow][column] = std::make_shared<TMS_Building>(*emptyBuilding);
        _buildingGrid[row][column]->setSpan({ _baseRect.x + column * _buildingWidth,
                                              _baseRect.y + upRow * _buildingHeight,
                                              _buildingWidth,
                                              _buildingHeight });
        _eventDispatcher.addEntity(_buildingGrid[upRow][column]);
    }
    if (downRow < ROW_NUM && _buildingGrid[downRow][column] == nullptr)
    {
        _buildingGrid[downRow][column] = std::make_shared<TMS_Building>(*emptyBuilding);
        _buildingGrid[row][column]->setSpan({ _baseRect.x + column * _buildingWidth,
                                              _baseRect.y + downRow * _buildingHeight,
                                              _buildingWidth,
                                              _buildingHeight });
        _eventDispatcher.addEntity(_buildingGrid[downRow][column]);
    }
    if (leftColumn >= 0 && _buildingGrid[row][leftColumn] == nullptr)
    {
        _buildingGrid[row][leftColumn] = std::make_shared<TMS_Building>(*emptyBuilding);
        _buildingGrid[row][column]->setSpan({ _baseRect.x + leftColumn * _buildingWidth,
                                              _baseRect.y + row * _buildingHeight,
                                              _buildingWidth,
                                              _buildingHeight });
        _eventDispatcher.addEntity(_buildingGrid[row][leftColumn]);
    }
    if (rightColumn < COLUMN_NUM && _buildingGrid[row][rightColumn] == nullptr)
    {
        _buildingGrid[row][rightColumn] = std::make_shared<TMS_Building>(*emptyBuilding);
        _buildingGrid[row][column]->setSpan({ _baseRect.x + rightColumn * _buildingWidth,
                                              _baseRect.y + row * _buildingHeight,
                                              _buildingWidth,
                                              _buildingHeight });
        _eventDispatcher.addEntity(_buildingGrid[row][rightColumn]);
    }

    return true;
}
