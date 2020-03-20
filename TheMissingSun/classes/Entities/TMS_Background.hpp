#ifndef TMS_BACKGROUND_HPP
#define TMS_BACKGROUND_HPP

#include "../../include/tms_shader_namespace.hpp"
#include "../../include/tms_texture_namespace.hpp"
#include "../TMS_Quad.hpp"
#include "TMS_Entity.hpp"

class TMS_Background : public TMS_Entity
{
public:
    enum class Shader
    {
        PLAIN,
        TOT
    };

    enum class Texture
    {
        SKY,
        SURFACE,
        UNDERGROUND,
        TOT
    };

    TMS_Background(std::vector<const TMS_Shader*>& shaders, std::vector<const TMS_Texture*>& textures,
                   const tms::Rect<float> span, const int winW, const int winH, const unsigned int id = 0);

    /***************** CONSTANTS *****************/
    static constexpr float LEVEL_HEIGHT_FRACTION = 0.5f;
    /* List of all required shaders. */
    static constexpr std::string_view REQUIRED_SHADERS[] =
    {
        tms::shader::NAMES[static_cast<int>(tms::shader::Name::PLAIN)]
    };
    /* List of all required textures. */
    static constexpr std::string_view REQUIRED_TEXTURES[] = 
    {
        tms::texture::NAMES[static_cast<int>(tms::texture::Name::SKY)],
        tms::texture::NAMES[static_cast<int>(tms::texture::Name::SURFACE)],
        tms::texture::NAMES[static_cast<int>(tms::texture::Name::UNDERGROUND)]
    }; 

    /***************** METHODS *****************/
    /* Base class methods. */
    std::string_view getName() const override;
    tms::Layer getLayer() const override;
    bool checkCollision(const float x, const float y) const override;
    std::vector<tms::EventType>& getRelevantEvents() override;
    void handleEvent(const SDL_Event& event) override;
    void render() override;
    void setSpan(tms::Rect<float> span) override;

private:
    /***************** METHODS *****************/
    void _setEvents(); // Set which events will be handled by the background.

    /***************** VARIABLES *****************/
    TMS_Quad _skyQuad, _surfaceQuad, _undergroundQuad;
    int _winW, _winH; // Window resolution.
};

#endif