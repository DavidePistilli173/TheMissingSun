#ifndef TMS_RESOURCECONTAINER_HPP
#define TMS_RESOURCECONTAINER_HPP

#include <map>
#include <memory>
#include <string>

/* Class for loading and storing a given resouce type. */
template <typename T>
class TMS_ResourceContainer
{
public:
    TMS_ResourceContainer() {};

    bool loadResources(const std::string file); // Load the resources listed in an xml configuration file.

    /* Get the resource with the specified id. */
    std::shared_ptr<T>& get(const std::string id)
    {
        return _data.find(id)->second;
    }

private:
    std::map<std::string, std::shared_ptr<T>> _data; // Stored resources.
};

#endif