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
                   tms::Rect span);
    TMS_Background(std::vector<std::shared_ptr<TMS_Shader>>& shaders, std::vector<std::shared_ptr<TMS_Texture>>& textures, 
                   unsigned int id, tms::Rect span);
    ~TMS_Background();
    TMS_Background(const TMS_Background& oldBackground) = delete;
    TMS_Background& operator= (const TMS_Background& oldBackground) = delete;
    TMS_Background(TMS_Background&& oldBackground) noexcept;
    TMS_Background& operator= (TMS_Background&& oldBackground) noexcept;

    /***************** CONSTANTS *****************/
    static constexpr float LEVEL_HEIGHT_FRACTION = 0.5f;
    static constexpr int BUFFER_SIZE = 20;
    static const std::string REQUIRED_SHADERS[];
    static const std::string REQUIRED_TEXTURES[];

    /***************** METHODS *****************/
    /* Base class methods. */
    tms::Layer getLayer() override;
    bool checkCollision(const int x, const int y) override;
    std::vector<tms::EventType>& getRelevantEvents() override;
    std::optional<TMS_Action> handleEvent(const SDL_Event& event) override;
    void render() override;

private:
    /***************** METHODS *****************/
    void _initBuffers(); // Initialise OpenGL buffers.
    void _setEvents(); // Set which events will be handled by the background.

    /***************** VARIABLES *****************/
    tms::Rect _skyRect, _surfaceRect, _undergroundRect; // Boxes for background levels.
    unsigned int _skyVAO, _skyVBO, _surfaceVAO, _surfaceVBO, _undergroundVAO, _undergroundVBO, _EBO; // OpenGL buffers.
};

#endif