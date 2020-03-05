#ifndef TMS_SHADER_NAMESPACE_HPP
#define TMS_SHADER_NAMESPACE_HPP

#include "tms.hpp"

namespace tms::shader
{
    /***************** RESOURCE LIST *****************/
    const std::string LIST_FILE = "./config/shaders.xml";
    constexpr char LIST_ROOT[] = "Shaders";
    constexpr char LIST_ELEMENT[] = "Shader";
    constexpr char LIST_ELEMENT_NAME[] = "Name";
    constexpr char LIST_ELEMENT_VERTEX[] = "VertexFile";
    constexpr char LIST_ELEMENT_FRAGMENT[] = "FragmentFile";
    constexpr char LIST_ELEMENT_UNIFORMS[] = "Uniforms";
    constexpr char LIST_ELEMENT_UNIFORM[] = "Uniform";

    /***************** ATTRIBUTE LOCATIONS *****************/
    enum class AttribLocation
    {
        VERTEX_COORDS, // Vertex coordinates.
        TEX_COORDS // Texture coordinates.
    };

    /***************** MENU SHADER PATHS *****************/
    /* Common. */
    const std::string PLAIN_VERTEX = "./shaders/common/plainVertex.glsl";
    const std::string PLAIN_FRAGMENT = "./shaders/common/plainFragment.glsl";
    const std::string HIGHLIGHT_FRAGMENT = "./shaders/common/highlightFragment.glsl";

    /***************** UNFIORMS *****************/
    /* Common. */
    enum class Plain
    {
        CAMERA_MATRIX,
        TEXTURE
    };
    constexpr const char* plain[] =
    {
        "cameraMatrix",
        "baseTexture"
    };
    enum class Highlight
    {
        CAMERA_MATRIX,
        TEXTURE
    };
    constexpr const char* highlight[] =
    {
        "cameraMatrix",
        "baseTexture"
    };

    /***************** GAME *****************/
    /* List of all game shaders. */
    enum class Name
    {
        PLAIN,
        HIGHLIGHT,
        TOT
    };
    /* List of shader names. */
    const std::string NAMES[] =
    {
        "plain",
        "highlight"
    };
}

#endif