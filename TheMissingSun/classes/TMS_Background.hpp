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
    TMS_Background(const TMS_Background& oldBackground) = delete;
    TMS_Background& operator= (const TMS_Background& oldBackground) = delete;
    // TODO: Add move constructor and move assignment operator.

    /***************** CONSTANTS *****************/
    static constexpr float LEVEL_HEIGHT_FRACTION = 0.5f;
    static constexpr int BUFFER_SIZE = 20;

    /***************** METHODS *****************/
    /* Base class methods. */
    tms::Layer getLayer();
    bool checkCollision(const int x, const int y);
    std::optional<TMS_Action> handleEvent(const TMS_Event& event);
    void render();

    /* Derived class methods. */
    void initBuffers();

private:
    tms::Rect _skyRect, _surfaceRect, _undergroundRect; // Boxes for background levels.
    unsigned int _skyVAO, _skyVBO, _surfaceVAO, _surfaceVBO, _undergroundVAO, _undergroundVBO, _EBO; // OpenGL buffers.
};

#endif