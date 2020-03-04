#include "../include/tinyxml2.h"
#include "../include/tms_shader_namespace.hpp"
#include "../include/tms_texture_namespace.hpp"
#include "TMS_ResourceContainer.hpp"
#include "TMS_Shader.hpp"
#include "TMS_Texture.hpp"

/* Shader specialisation. */
template<>
bool TMS_ResourceContainer<TMS_Shader>::loadResources(const std::string file)
{
    /* Load the list of resources. */
    tinyxml2::XMLDocument resourceList;
    resourceList.LoadFile(file.c_str());
    if (resourceList.Error())
    {
        printf("Unable to load %s.\n", file.c_str());
        printf("%s\n", resourceList.ErrorIDToName(resourceList.ErrorID()));
        return false;
    }

    /* Parse the list and load all resources. */
    /* Find the root element. */
    tinyxml2::XMLElement* root = resourceList.FirstChildElement(tms::shader::LIST_ROOT);
    if (root == nullptr)
    {
        printf("Wrong format for file %s.\n", file.c_str());
        return false;
    }
    /* Find the first shader. */
    tinyxml2::XMLElement* shader = root->FirstChildElement(tms::shader::LIST_ELEMENT);
    if (shader == nullptr)
    {
        printf("No shaders found in file %s.\n", file.c_str());
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
bool TMS_ResourceContainer<TMS_Texture>::loadResources(const std::string file)
{
    /* Load the list of resources. */
    tinyxml2::XMLDocument resourceList;
    resourceList.LoadFile(file.c_str());
    if (resourceList.Error())
    {
        printf("Unable to load %s.\n", file.c_str());
        printf("%s\n", resourceList.ErrorIDToName(resourceList.ErrorID()));
        return false;
    }

    /* Parse the list and load all resources. */
    /* Find the root element. */
    tinyxml2::XMLElement* root = resourceList.FirstChildElement(tms::texture::LIST_ROOT);
    if (root == nullptr)
    {
        printf("Wrong format for file %s.\n", file.c_str());
        return false;
    }
    /* Find the first texture. */
    tinyxml2::XMLElement* texture = root->FirstChildElement(tms::texture::LIST_ELEMENT);
    if (texture == nullptr)
    {
        printf("No textures found in file %s.\n", file.c_str());
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