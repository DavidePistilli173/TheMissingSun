#include "../include/tinyxml2.h"
#include "../include/tms_building_namespace.hpp"
#include "../include/tms_shader_namespace.hpp"
#include "../include/tms_texture_namespace.hpp"
#include "./Entities/TMS_Building.hpp"
#include "TMS_ResourceContainer.hpp"

/***************** HELPER FUNCTIONS *****************/
/* Set a cost list for a building. */
std::optional<TMS_Item> getItem(tinyxml2::XMLElement* itemRoot, const std::string_view buildingName, 
                                const TMS_ResourceContainer<TMS_Texture>& textures)
{
    TMS_Item item;

    /* Set the item's name. */
    if (auto name = itemRoot->FirstChildElement(tms::building::LIST_NAME);
        name == nullptr)
    {
        printf("Missing item name in the cost list for building %s.\n", buildingName.data());
        return std::optional<TMS_Item>();
    }
    else
    {
        item.setName(name->FirstChild()->Value());
    }

    /* Set the item's amount. */
    if (auto amount = itemRoot->FirstChildElement(tms::building::LIST_AMOUNT);
        amount == nullptr)
    {
        printf("Missing item amount in the cost list for building %s.\n", buildingName.data());
        return std::optional<TMS_Item>();
    }
    else
    {
        item.setAmount(std::atoi(amount->FirstChild()->Value()));
    }

    /* Set the item's texture. */
    if (auto texture = textures.get(item.getName());
        texture == nullptr)
    {
        /* Set the default texture. */
        item.setTexture(textures.get(tms::texture::NAMES[static_cast<int>(tms::texture::Name::ITEM_DEFAULT)]));
    }
    else
    {
        /* Set the specified texture. */
        item.setTexture(texture);
    }

    return std::optional<TMS_Item>(item);
}

std::optional<std::pair<TMS_Item, std::vector<TMS_Item>>> getProductionItem(tinyxml2::XMLElement* itemRoot, const std::string_view buildingName,
                                                                            const TMS_ResourceContainer<TMS_Texture>& textures)
{
    /* Get the item to be produced. */
    std::optional<TMS_Item> item = getItem(itemRoot, buildingName, textures);
    if (!item.has_value()) return std::optional<std::pair<TMS_Item, std::vector<TMS_Item>>>();

    /* Get the cost for that item, if present. */
    std::vector<TMS_Item> costList;
    if (auto costListRoot = itemRoot->FirstChildElement(tms::building::LIST_COST);
        costListRoot != nullptr)
    {
        auto costItemRoot = costListRoot->FirstChildElement(tms::building::LIST_ITEM);
        if (costItemRoot == nullptr)
        {
            printf("Empty cost list for building %s.\n", buildingName.data());
            return std::optional<std::pair<TMS_Item, std::vector<TMS_Item>>>();
        }
        while (costItemRoot != nullptr)
        {
            std::optional<TMS_Item> costItem = getItem(costItemRoot, buildingName, textures);
            if (!costItem.has_value()) return std::optional<std::pair<TMS_Item, std::vector<TMS_Item>>>();
            costList.push_back(costItem.value());
            costItemRoot = costItemRoot->NextSiblingElement();
        }
    }

    return std::optional<std::pair<TMS_Item, std::vector<TMS_Item>>>(std::make_pair(item.value(), costList));
}

/* Shader specialisation. */
template<>
bool TMS_ResourceContainer<TMS_Shader>::loadResources(const std::string_view file)
{
    /* Load the list of resources. */
    tinyxml2::XMLDocument resourceList;
    resourceList.LoadFile(file.data());
    if (resourceList.Error())
    {
        printf("Unable to load %s.\n", file.data());
        printf("%s\n", resourceList.ErrorIDToName(resourceList.ErrorID()));
        return false;
    }

    /* Parse the list and load all resources. */
    /* Find the root element. */
    tinyxml2::XMLElement* root = resourceList.FirstChildElement(tms::shader::LIST_ROOT);
    if (root == nullptr)
    {
        printf("Wrong format for file %s.\n", file.data());
        return false;
    }
    /* Find the first shader. */
    tinyxml2::XMLElement* shader = root->FirstChildElement(tms::shader::LIST_ELEMENT);
    if (shader == nullptr)
    {
        printf("No shaders found in file %s.\n", file.data());
        return false;
    }

    /* Load all shaders. */
    while (shader != nullptr)
    {
        /* Load the shader's name. */
        std::string name = shader->FirstChildElement(tms::shader::LIST_ELEMENT_NAME)->FirstChild()->Value();
        /* Load the shader's files. */
        std::string vertexFile = shader->FirstChildElement(tms::shader::LIST_ELEMENT_VERTEX)->FirstChild()->Value();
        std::string fragmentFile = shader->FirstChildElement(tms::shader::LIST_ELEMENT_FRAGMENT)->FirstChild()->Value();

        /* Create the shader. */
        std::shared_ptr<TMS_Shader> shaderPtr;
        try
        {
            shaderPtr = std::make_shared<TMS_Shader>(TMS_Shader(vertexFile, fragmentFile));
        }
        catch (std::string e)
        {
            printf("Unable to load shader %s.\n%s", name.c_str(), e.c_str());
            return false;
        }

        /* Add uniforms to the shader. */
        tinyxml2::XMLElement* uniformRoot = shader->FirstChildElement(tms::shader::LIST_ELEMENT_UNIFORMS);
        tinyxml2::XMLElement* uniform = uniformRoot->FirstChildElement(tms::shader::LIST_ELEMENT_UNIFORM);

        while (uniform != nullptr)
        {
            std::string uniformName = uniform->FirstChild()->Value();
            if (!shaderPtr->addUniform(uniformName.c_str()))
            {
                printf("Wrong uniform %s for shader %s.\n", uniformName.c_str(), name.c_str());
                return false;
            }
            uniform = uniform->NextSiblingElement();
        }

        /* Add the shader to the container. */
        _data.insert(std::pair<std::string, std::shared_ptr<TMS_Shader>>(name, shaderPtr));

        shader = shader->NextSiblingElement();
    }

    return true;
}

/* Texture specialisation. */
template<>
bool TMS_ResourceContainer<TMS_Texture>::loadResources(const std::string_view file)
{
    /* Load the list of resources. */
    tinyxml2::XMLDocument resourceList;
    resourceList.LoadFile(file.data());
    if (resourceList.Error())
    {
        printf("Unable to load %s.\n", file.data());
        printf("%s\n", resourceList.ErrorIDToName(resourceList.ErrorID()));
        return false;
    }

    /* Parse the list and load all resources. */
    /* Find the root element. */
    tinyxml2::XMLElement* root = resourceList.FirstChildElement(tms::texture::LIST_ROOT);
    if (root == nullptr)
    {
        printf("Wrong format for file %s.\n", file.data());
        return false;
    }
    /* Find the first texture. */
    tinyxml2::XMLElement* texture = root->FirstChildElement(tms::texture::LIST_ELEMENT);
    if (texture == nullptr)
    {
        printf("No textures found in file %s.\n", file.data());
        return false;
    }

    /* Load all textures. */
    while (texture != nullptr)
    {
        /* Read texture data. */
        std::string name = texture->FirstChildElement(tms::texture::LIST_ELEMENT_NAME)->FirstChild()->Value();
        std::string file = texture->FirstChildElement(tms::texture::LIST_ELEMENT_FILE)->FirstChild()->Value();
        /* Create the texture and add it to the container. */
        std::shared_ptr<TMS_Texture> texturePtr;
        try
        {
            texturePtr = std::make_shared<TMS_Texture>(TMS_Texture(file));
            _data.insert(std::pair<std::string, std::shared_ptr<TMS_Texture>>(name, texturePtr));
        }
        catch (std::string e)
        {
            printf("Unable to load texture %s.\n%s", name.c_str(), e.c_str());
            return false;
        }

        texture = texture->NextSiblingElement();
    }

    return true;
}

/* Building specialisation. */
template<>
bool TMS_ResourceContainer<TMS_Building>::loadResources(const std::string_view file, const TMS_ResourceContainer<TMS_Shader> shaders, 
                                                        const TMS_ResourceContainer<TMS_Texture>& textures)
{
    /* Load the file into memory. */
    tinyxml2::XMLDocument buildingList;
    buildingList.LoadFile(file.data());
    if (buildingList.Error())
    {
        printf("Unable to load %s.\n", file.data());
        printf("%s\n", buildingList.ErrorIDToName(buildingList.ErrorID()));
        return false;
    }

    /* Find the root element. */
    tinyxml2::XMLElement* root = buildingList.FirstChildElement(tms::building::LIST_ROOT);
    if (root == nullptr)
    {
        printf("Wrong format for file %s.\n", file.data());
        return false;
    }

    /* Read all building types. */
    for (tinyxml2::XMLElement* building = root->FirstChildElement(tms::building::LIST_ELEMENT); 
         building != nullptr;
         building = building->NextSiblingElement())
    {
        TMS_Building currentBuilding;

        /* Set the building's shaders. */
        std::vector<std::shared_ptr<TMS_Shader>> requiredShaders;
        for (int i = 0; i < static_cast<int>(TMS_Building::Shader::TOT); ++i)
        {
            if (requiredShaders.emplace_back(shaders.get(TMS_Building::REQUIRED_SHADERS[i])) == nullptr)
            {
                printf("Unable to find shader %s.\n", TMS_Building::REQUIRED_SHADERS[i].data());
                return false;
            }
        }
        currentBuilding.setShaders(requiredShaders);

        /* Get the building's name. */
        std::string buildingName;
        if (auto name = building->FirstChildElement(tms::building::LIST_NAME);
            building == nullptr)
        {
            printf("Missing building name in file %s.\n", file.data());
            return false;
        }
        else
        {
            buildingName = name->FirstChild()->Value();
        }
        currentBuilding.setName(buildingName);

        /* Get the building's build time. */
        int buildTime;
        if (auto time = building->FirstChildElement(tms::building::LIST_BUILD_TIME);
            time == nullptr)
        {
            printf("Missing build time from building %s.\n", buildingName.c_str());
            return false;
        }
        else
        {
            buildTime = std::atoi(time->FirstChild()->Value());
        }
        if (!currentBuilding.setBuildTime(buildTime))
        {
            printf("Negative build time for building %s.\n", buildingName.c_str());
            return false;
        }

        /* Get the building's cost, if present. */
        if (auto costRoot = building->FirstChildElement(tms::building::LIST_COST);
            costRoot != nullptr)
        {
            auto costItem = costRoot->FirstChildElement(tms::building::LIST_ITEM);
            if (costItem == nullptr)
            {
                printf("Empty cost list for building %s.\n", currentBuilding.getName().c_str());
                return false;
            }
            while (costItem != nullptr)
            {
                std::optional<TMS_Item> item = getItem(costItem, currentBuilding.getName(), textures);
                if (!item.has_value()) return false;
                currentBuilding.addBuildCost(item.value());
                costItem = costItem->NextSiblingElement();
            }
        }

        /* Get the building's fixed production, if present. */
        if (auto fixedProductionRoot = building->FirstChildElement(tms::building::LIST_FIXED_PROD);
            fixedProductionRoot != nullptr)
        {
            auto itemRoot = fixedProductionRoot->FirstChildElement(tms::building::LIST_ITEM);
            if (itemRoot == nullptr)
            {
                printf("Empty fixed production list for building %s.\n", currentBuilding.getName().c_str());
                return false;
            }
            while (itemRoot != nullptr)
            {
                std::optional<std::pair<TMS_Item, std::vector<TMS_Item>>> productionItem =
                    getProductionItem(itemRoot, currentBuilding.getName(), textures);
                if (!productionItem.has_value()) return false;

                currentBuilding.addFixedProduction(productionItem.value().first, productionItem.value().second);
                itemRoot = itemRoot->NextSiblingElement();
            }
        }

        /* Get the building's continuous production, if present. */
        if (auto continuousProductionRoot = building->FirstChildElement(tms::building::LIST_CONT_PROD);
            continuousProductionRoot != nullptr)
        {
            auto itemRoot = continuousProductionRoot->FirstChildElement(tms::building::LIST_ITEM);
            if (itemRoot == nullptr)
            {
                printf("Empty continuous production list for building %s.\n", currentBuilding.getName().c_str());
                return false;
            }
            while (itemRoot != nullptr)
            {
                std::optional<std::pair<TMS_Item, std::vector<TMS_Item>>> productionItem =
                    getProductionItem(itemRoot, currentBuilding.getName(), textures);
                if (!productionItem.has_value()) return false;

                currentBuilding.addContinuousProduction(productionItem.value().first, productionItem.value().second);
                itemRoot = itemRoot->NextSiblingElement();
            }
        }

        /* Get the building's one-time production, if present. */
        if (auto oneTimeProductionRoot = building->FirstChildElement(tms::building::LIST_OT_PROD);
            oneTimeProductionRoot != nullptr)
        {
            auto itemRoot = oneTimeProductionRoot->FirstChildElement(tms::building::LIST_ITEM);
            if (itemRoot == nullptr)
            {
                printf("Empty one-time production list for building %s.\n", currentBuilding.getName().c_str());
                return false;
            }
            while (itemRoot != nullptr)
            {
                std::optional<std::pair<TMS_Item, std::vector<TMS_Item>>> productionItem =
                    getProductionItem(itemRoot, currentBuilding.getName(), textures);
                if (!productionItem.has_value()) return false;

                currentBuilding.addOneTimeProduction(productionItem.value().first, productionItem.value().second);
                itemRoot = itemRoot->NextSiblingElement();
            }
        }

        /* Get the building's storage, if present. */
        if (auto storageRoot = building->FirstChildElement(tms::building::LIST_STORAGE);
            storageRoot != nullptr)
        {
            auto itemRoot = storageRoot->FirstChildElement(tms::building::LIST_ITEM);
            if (itemRoot == nullptr)
            {
                printf("Empty storage list for building %s.\n", currentBuilding.getName().c_str());
                return false;
            }
            while (itemRoot != nullptr)
            {
                std::optional<TMS_Item> item = getItem(itemRoot, currentBuilding.getName(), textures);
                if (!item.has_value()) return false;

                int capacity = item.value().getAmount();
                item.value().setAmount(0);
                currentBuilding.addStorage(item.value(), capacity);
                itemRoot = itemRoot->NextSiblingElement();
            }
        }

        /* Get the building's textures. */
        if (auto textureListRoot = building->FirstChildElement(tms::building::LIST_TEXTURES);
            textureListRoot != nullptr)
        {
            auto textureElement = textureListRoot->FirstChildElement(tms::building::LIST_TEXTURE_ELEMENT);
            if (textureElement == nullptr)
            {
                printf("Empty texture list for building %s.\n", currentBuilding.getName().c_str());
                return false;
            }
            while (textureElement != nullptr)
            {
                if (auto texture = textures.get(textureElement->FirstChild()->Value());
                    texture == nullptr)
                {
                    /* Set the default texture. */
                    currentBuilding.addTexture(textures.get(tms::texture::NAMES[static_cast<int>(tms::texture::Name::ITEM_DEFAULT)]));
                }
                else
                {
                    /* Add the specified texture. */
                    currentBuilding.addTexture(texture);
                }
                textureElement = textureElement->NextSiblingElement();
            }
        }

        /* Add the building to the container. */
        _data.insert(std::make_pair(currentBuilding.getName(), std::make_shared<TMS_Building>(currentBuilding)));
    }

    return true;
}