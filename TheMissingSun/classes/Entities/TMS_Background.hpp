#ifndef TMS_BACKGROUND_HPP
#define TMS_BACKGROUND_HPP

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
                   const tms::Rect span, const int winW, const int winH);
    TMS_Background(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures, 
                   const unsigned int id, const tms::Rect span, const int winW, const int winH);
    ~TMS_Background();
    TMS_Background(const TMS_Background& oldBackground) = delete;
    TMS_Background& operator= (const TMS_Background& oldBackground) = delete;
    TMS_Background(TMS_Background&& oldBackground) noexcept;
    TMS_Background& operator= (TMS_Background&& oldBackground) noexcept;

    /***************** CONSTANTS *****************/
    static constexpr float LEVEL_HEIGHT_FRACTION = 0.5f;
    static constexpr int BUFFER_SIZE = 20;
    static const std::string REQUIRED_SHADERS[]; // List of all required shaders.
    static const std::string REQUIRED_TEXTURES[]; // List of all required textures.

    /***************** METHODS *****************/
    /* Base class methods. */
    tms::Layer getLayer() const override;
    bool checkCollision(const int x, const int y) const override;
    std::vector<tms::EventType>& getRelevantEvents() override;
    void handleEvent(const SDL_Event& event) override;
    void render() override;

private:
    /***************** METHODS *****************/
    void _initBuffers(); // Initialise OpenGL buffers.
    void _setEvents(); // Set which events will be handled by the background.

    /***************** VARIABLES *****************/
    tms::Rect _skyRect, _surfaceRect, _undergroundRect; // Boxes for background levels.
    unsigned int _skyVAO, _skyVBO, _surfaceVAO, _surfaceVBO, _undergroundVAO, _undergroundVBO, _EBO; // OpenGL buffers.
    int _winW, _winH; // Window resolution.
};

#endif