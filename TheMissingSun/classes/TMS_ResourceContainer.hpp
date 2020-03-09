#ifndef TMS_RESOURCECONTAINER_HPP
#define TMS_RESOURCECONTAINER_HPP

#include <map>
#include <memory>
#include <string>

#include "TMS_Shader.hpp"
#include "TMS_Texture.hpp"

/* Class for loading and storing a given resouce type. */
template <typename T>
class TMS_ResourceContainer
{
public:
    TMS_ResourceContainer() {};

    bool loadResources(const std::string file); // Load the resources listed in an xml configuration file.
    /* Load resources which require shaders and textures. */
    bool loadResources(const std::string file, const TMS_ResourceContainer<TMS_Texture>& textures);

    /* Get the resource with the specified id. */
    std::shared_ptr<T> get(const std::string id) const
    {
        auto requiredData = _data.find(id);
        if (requiredData == _data.end()) return nullptr;
        return _data.find(id)->second;
    }

private:
    std::map<std::string, std::shared_ptr<T>> _data; // Stored resources.
};

#endif