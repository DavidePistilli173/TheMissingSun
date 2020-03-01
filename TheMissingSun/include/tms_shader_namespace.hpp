#ifndef TMS_SHADER_NAMESPACE_HPP
#define TMS_SHADER_NAMESPACE_HPP

#include "tms.hpp"

namespace tms::shader
{
    /***************** ATTRIBUTE LOCATIONS *****************/
    enum class AttribLocation
    {
        VERTEX_COORDS, // Vertex coordinates.
        TEX_COORDS // Texture coordinates.
    };

    /***************** SHADER PATHS *****************/
    /* Common. */
    const std::string PLAIN_VERTEX = "./shaders/common/plainVertex.glsl";
    const std::string PLAIN_FRAGMENT = "./shaders/common/plainFragment.glsl";

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

    /***************** GAME *****************/
    /* List of all game shaders. */
    enum class Name
    {
        PLAIN,
        TOT
    };
    /* List of all files containing game shaders. */
    const std::string FILES[] =
    {
        "./shaders/common/plainVertex.glsl",
        "./shaders/common/plainFragment.glsl"
    };
    /* List of shader names. */
    const std::string NAMES[] =
    {
        "plain"
    };
}

#endif