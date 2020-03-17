#ifndef TMS_BACKGROUND_HPP
#define TMS_BACKGROUND_HPP

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

    TMS_Background(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures,
                   const tms::Rect<float> span, const int winW, const int winH, const unsigned int id = 0);

    /***************** CONSTANTS *****************/
    static constexpr float LEVEL_HEIGHT_FRACTION = 0.5f;
    static const std::string REQUIRED_SHADERS[]; // List of all required shaders.
    static const std::string REQUIRED_TEXTURES[]; // List of all required textures.

    /***************** METHODS *****************/
    /* Base class methods. */
    tms::Layer getLayer() const override;
    bool checkCollision(const float x, const float y) const override;
    std::vector<tms::EventType>& getRelevantEvents() override;
    void handleEvent(const SDL_Event& event) override;
    void render() override;

private:
    /***************** METHODS *****************/
    void _setEvents(); // Set which events will be handled by the background.

    /***************** VARIABLES *****************/
    TMS_Quad _skyQuad, _surfaceQuad, _undergroundQuad;
    int _winW, _winH; // Window resolution.
};

#endif