#ifndef TMS_BUILDING_NAMESPACE_HPP
#define TMS_BUILDING_NAMESPACE_HPP

#include "tms.hpp"

namespace tms::building
{
    const std::string BUILDING_TYPES_FILE = "./config/buildings.xml"; // Configuration file for building types.
    const std::string INITIAL_BUILDING = "Entrance"; // First building present in the base.
    const std::string EMPTY_BUILDING = "Empty"; // Empty building slot.

    /* Different frequencies for item production/consumption. */
    enum class Frequency
    {
        FIXED, // Fixed change in resource amount while the building is active.
        CONTINUOUS, // Per-tick change in resource amount.
        ONE_TIME // Single change in resource amount.
    };

    /***************** XML TAGS *****************/
    constexpr char LIST_ROOT[] = "Buildings";
    constexpr char LIST_ELEMENT[] = "Building";
    constexpr char LIST_NAME[] = "Name";
    constexpr char LIST_BUILD_TIME[] = "BuildTime";
    constexpr char LIST_COST[] = "Cost";
    constexpr char LIST_ITEM[] = "Item";
    constexpr char LIST_AMOUNT[] = "Amount";
    constexpr char LIST_FIXED_PROD[] = "FixedProduction";
    constexpr char LIST_CONT_PROD[] = "ContinuousProduction";
    constexpr char LIST_OT_PROD[] = "OneTimeProduction";
    constexpr char LIST_STORAGE[] = "Storage";
    constexpr char LIST_TEXTURES[] = "Textures";
    constexpr char LIST_TEXTURE_ELEMENT[] = "Texture";
}

#endif