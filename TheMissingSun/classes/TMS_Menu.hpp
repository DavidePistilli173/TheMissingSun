#ifndef TMS_MENU_HPP
#define TMS_MENU_HPP

#include <vector>

#include "../include/tms.hpp"
#include "TMS_Clock.hpp"
#include "TMS_MenuPage.hpp"
#include "TMS_Shader.hpp"
#include "TMS_Texture.hpp"

/* Menu class. */
class TMS_Menu
{
public:
    /* Shaders. */
    enum class Shader
    {
        PLAIN, // Plain shader, just renders the textures.
        TOTAL
    };
    /* Textures. */
    enum class Texture
    {
        BACKGROUND,
        BUTTON,
        TOTAL
    };

    TMS_Menu();

    /**************** METHODS ****************/
    /* Initialise all menu data. */
    bool init(const int windowWidth, const int windowHeight);
    /* Render the menu. */
    void render(tms::window_t& window, const int windowWidth, const int windowHeight);
    /* Main menu loop. */
    tms::GameState menuLoop();

private:
    /**************** METHODS ****************/
    /* Load menu layout. */
    bool _loadLayout(const int windowWidth, const int windowHeight);
    /* Load menu shaders. */
    bool _loadShaders();
    /* Load menu textures. */
    bool _loadTextures();
    /* Load OpenGL buffers. */
    void _loadVAO(const int windowWidth, const int windowHeight);

    TMS_Clock _clock; // Input timer.
    std::shared_ptr<TMS_MenuPage> _currentPage; // Current menu page.
    std::vector<std::shared_ptr<TMS_MenuPage>> _pages; // All menu pages.
    tms::GameState _menuState; // Used to signal menu exit.
    std::vector<TMS_Shader> _shaders; // Vector with all shaders.
    std::vector<TMS_Texture> _textures; // Vector with all textures.

    /* Vertex Array Objects. */
    GLuint _backgroundVAO; // VAO for menu background.
    GLuint _backgroundVBO; // VBO for menu background.
    GLuint _backgroundEBO; // EBO for menu background.
};

#endif