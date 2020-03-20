#ifndef TMS_RESOURCECONTAINER_HPP
#define TMS_RESOURCECONTAINER_HPP

#include <map>
#include <memory>
#include <string>
#include <string_view>

#include "TMS_Shader.hpp"
#include "TMS_Texture.hpp"

/* Class for loading and storing a given resouce type. */
template <typename T>
class TMS_ResourceContainer
{
public:
    TMS_ResourceContainer() {};

    bool loadResources(const std::string_view file); // Load the resources listed in an xml configuration file.
    /* Load resources which require textures. */
    bool loadResources(const std::string_view file, const TMS_ResourceContainer<TMS_Shader>& shaders, 
                       const TMS_ResourceContainer<TMS_Texture>& textures);

    /* Get the resource with the specified id. */
    const T* get(const std::string_view id) const
    {
        auto requiredData = _data.find(id.data());
        if (requiredData == _data.end()) return nullptr;
        return requiredData->second.get();
    }

private:
    std::map<std::string, std::unique_ptr<T>> _data; // Stored resources.
};

#endif