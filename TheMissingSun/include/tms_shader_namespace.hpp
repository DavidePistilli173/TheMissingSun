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
        CAMERA_MATRIX
    };
    constexpr const char* plain[] =
    {
        "cameraMatrix"
    };
}

#endif