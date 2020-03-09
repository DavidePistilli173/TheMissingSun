#include "TMS_Building.hpp"

TMS_Building::TMS_Building(tms::Rect span) :
    TMS_Entity(),
    _name(""),
    _buildTime(0),
    _span(span)
{
}

tms::Layer TMS_Building::getLayer() const
{
    return tms::default_layer();
}

std::vector<tms::EventType>& TMS_Building::getRelevantEvents()
{
    return _relevantEvents;
}

bool TMS_Building::checkCollision(const int x, const int y) const
{
    if (x >= _span.x && x <= _span.x + _span.w &&
        y >= _span.y && y <= _span.y + _span.h) return true;
    return false;
}

std::optional<TMS_Action> TMS_Building::handleEvent(const SDL_Event& event)
{
    return std::optional<TMS_Action>();
}

void TMS_Building::render()
{
}

const std::string& TMS_Building::getName() const
{
    return _name;
}

void TMS_Building::setName(const std::string& name)
{
    _name = name;
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

void TMS_Building::addTexture(const std::shared_ptr<TMS_Texture>& texture)
{
    _textures.push_back(texture);
}
