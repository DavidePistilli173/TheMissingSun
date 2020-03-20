#include "../../include/tms_shader_namespace.hpp"
#include "TMS_Building.hpp"

TMS_Building::TMS_Building(const TMS_Texture* noTexture) :
    TMS_Entity(),
    _drawSprite(_span, this->getLayer(), noTexture, GL_STATIC_DRAW)
{
    /* Set relevant events. */
    _relevantEvents.push_back(tms::EventType::MOUSE_LEFT_CLICK);
    _relevantEvents.push_back(tms::EventType::MOUSE_HOVER);
}

TMS_Building::TMS_Building(const TMS_Building& oldBuilding) :
    TMS_Entity(oldBuilding),
    _drawSprite(_span, this->getLayer(), _textures[0], GL_STATIC_DRAW)
{
    /* Copy basic building information. */
    _name = oldBuilding._name;
    _buildTime = oldBuilding._buildTime;
    _buildCost = oldBuilding._buildCost;
    _fixedProduction = oldBuilding._fixedProduction;
    _continuousProduction = oldBuilding._continuousProduction;
    _oneTimeProduction = oldBuilding._oneTimeProduction;
    _storage = oldBuilding._storage;
    _timeStep = oldBuilding._timeStep;

    /* Reset the building's storage. */
    for (auto container : _storage)
    {
        container.first.setAmount(0);
    }

    /* Reset the building's unique properties. */
    _selected = false;
    _highlighted = false;
    _currenTexture = 0;
}

TMS_Building& TMS_Building::operator=(const TMS_Building& oldBuilding)
{
    if (this != &oldBuilding)
    {
        TMS_Entity::operator=(oldBuilding);

        /* Copy basic building information. */
        _name = oldBuilding._name;
        _buildTime = oldBuilding._buildTime;
        _buildCost = oldBuilding._buildCost;
        _fixedProduction = oldBuilding._fixedProduction;
        _continuousProduction = oldBuilding._continuousProduction;
        _oneTimeProduction = oldBuilding._oneTimeProduction;
        _storage = oldBuilding._storage;
        _timeStep = oldBuilding._timeStep;

        /* Reset the building's storage. */
        for (auto container : _storage)
        {
            container.first.setAmount(0);
        }
        /* Reset the building's unique properties. */
        _span = { 0,0,0,0 };
        _drawSprite = TMS_Sprite(_span, this->getLayer(), _textures[0], GL_STATIC_DRAW);
        _selected = false;
        _highlighted = false;
        _currenTexture = 0;
    }
    return *this;
}

tms::Layer TMS_Building::getLayer() const
{
    return tms::default_layer();
}

std::vector<tms::EventType>& TMS_Building::getRelevantEvents()
{
    return _relevantEvents;
}

bool TMS_Building::checkCollision(const float x, const float y) const
{
    if (x >= _span.x && x <= _span.x + _span.w &&
        y >= _span.y && y <= _span.y + _span.h) return true;
    return false;
}

void TMS_Building::handleEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT) _selected = true;
        break;
    case SDL_MOUSEMOTION:
        _highlighted = true;
        break;
    }
}

void TMS_Building::render()
{
    /* Change animation frame if enough time has passed. */
    if (_clock.getTime() >= _timeStep)
    {
        _clock.startClock();
        _currenTexture = (_currenTexture + 1) % _textures.size();
    }

    /* Choose the proper shader. */
    if (_selected || _highlighted) _shaders[static_cast<int>(Shader::HIGHLIGHT)]->use();
    else _shaders[static_cast<int>(Shader::PLAIN)]->use();

    /* Render the building. */
    _drawSprite.setTexture(_textures[_currenTexture]);
    _drawSprite.draw();
}

std::string_view TMS_Building::getName() const
{
    return _name;
}

void TMS_Building::setName(const std::string& name)
{
    _name = name;
}

void TMS_Building::setSpan(const tms::Rect<float> span)
{
    /* Update the building's span. */
    _span = span;
    _drawSprite.setSpan(span);
}

bool TMS_Building::setShaders(const std::vector<const TMS_Shader*>& shaders)
{
    if (shaders.size() != static_cast<int>(Shader::TOT)) return false;
    _shaders = shaders;
    return true;
}

bool TMS_Building::setBuildTime(const int buildTime)
{
    if (buildTime < 0) return false;
    _buildTime = buildTime;
    return true;
}

void TMS_Building::addBuildCost(const TMS_Item& item)
{
    _buildCost.push_back(item);
}

void TMS_Building::addFixedProduction(const TMS_ItemProduction& production, const std::vector<TMS_ItemCost>& cost)
{
    _fixedProduction.push_back(std::make_pair(production, cost));
}

void TMS_Building::addContinuousProduction(const TMS_ItemProduction& production, const std::vector<TMS_ItemCost>& cost)
{
    _continuousProduction.push_back(std::make_pair(production, cost));
}

void TMS_Building::addOneTimeProduction(const TMS_ItemProduction& production, const std::vector<TMS_ItemCost>& cost)
{
    _oneTimeProduction.push_back(std::make_pair(production, cost));
}

bool TMS_Building::addStorage(const TMS_Item& item, const int maxCapacity)
{
    if (maxCapacity < 0) return false;
    _storage.push_back(std::make_pair(item, maxCapacity));
    return true;
}

void TMS_Building::addTexture(const TMS_Texture* texture)
{
    _textures.push_back(texture);
    _timeStep = static_cast<int>(static_cast<float>(ANIMATION_TIME) / _textures.size());
}

void TMS_Building::nSelect()
{
    _selected = false;
}

void TMS_Building::nHighlight()
{
    _highlighted = false;
}
