#include "../../include/tms_shader_namespace.hpp"
#include "TMS_Building.hpp"

/* Defitions of static variables. */
const std::string TMS_Building::REQUIRED_SHADERS[] =
{
    tms::shader::NAMES[static_cast<int>(tms::shader::Name::PLAIN)],
    tms::shader::NAMES[static_cast<int>(tms::shader::Name::HIGHLIGHT)]
};

TMS_Building::TMS_Building() :
    TMS_Entity(),
    _name(""),
    _buildTime(0),
    _span({0,0,0,0}),
    _highlighted(false)
{
    /* Create OpenGL buffers. */
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);
    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    /* Set default buffer data. */
    float vboData[] =
    {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vboData), vboData, GL_STATIC_DRAW);

    int strideSize = 5 * sizeof(float);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS), 3, GL_FLOAT, GL_FALSE, strideSize, 0);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS), 2, GL_FLOAT, GL_FALSE, strideSize, reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS));

    glGenBuffers(1, &_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tms::EBO_QUAD_DATA), tms::EBO_QUAD_DATA, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TMS_Building::~TMS_Building()
{
    glDeleteBuffers(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
}

TMS_Building::TMS_Building(const TMS_Building& oldBuilding) :
    TMS_Entity(oldBuilding)
{
    /* Copy basic building information. */
    _name = oldBuilding._name;
    _buildTime = oldBuilding._buildTime;
    _buildCost = oldBuilding._buildCost;
    _fixedProduction = oldBuilding._fixedProduction;
    _continuousProduction = oldBuilding._continuousProduction;
    _oneTimeProduction = oldBuilding._oneTimeProduction;
    _storage = oldBuilding._storage;

    /* Reset the building's storage. */
    for (auto container : _storage)
    {
        container.first.setAmount(0);
    }

    /* Reset the building's unique properties. */
    _span = { 0,0,0,0 };
    
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);
    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    /* Set default buffer data. */
    float vboData[] =
    {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vboData), vboData, GL_STATIC_DRAW);

    int strideSize = 5 * sizeof(float);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS), 3, GL_FLOAT, GL_FALSE, strideSize, 0);
    glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS), 2, GL_FLOAT, GL_FALSE, strideSize, reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS));
    glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS));

    glGenBuffers(1, &_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tms::EBO_QUAD_DATA), tms::EBO_QUAD_DATA, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    _highlighted = false;
}

TMS_Building& TMS_Building::operator=(const TMS_Building& oldBuilding)
{
    if (this != &oldBuilding)
    {
        TMS_Entity::operator=(oldBuilding);

        /* Copy basic building information. */
        _name = oldBuilding._name;
        _buildTime = oldBuilding._buildTime;
        _buildCost = oldBuilding._buildCost;
        _fixedProduction = oldBuilding._fixedProduction;
        _continuousProduction = oldBuilding._continuousProduction;
        _oneTimeProduction = oldBuilding._oneTimeProduction;
        _storage = oldBuilding._storage;

        /* Reset the building's storage. */
        for (auto container : _storage)
        {
            container.first.setAmount(0);
        }
        /* Reset the building's unique properties. */
        _span = { 0,0,0,0 };
        glGenVertexArrays(1, &_VAO);
        glBindVertexArray(_VAO);
        glGenBuffers(1, &_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);

        /* Set default buffer data. */
        float vboData[] =
        {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vboData), vboData, GL_STATIC_DRAW);

        int strideSize = 5 * sizeof(float);
        glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS), 3, GL_FLOAT, GL_FALSE, strideSize, 0);
        glVertexAttribPointer(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS), 2, GL_FLOAT, GL_FALSE, strideSize, reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::VERTEX_COORDS));
        glEnableVertexAttribArray(static_cast<int>(tms::shader::AttribLocation::TEX_COORDS));

        glGenBuffers(1, &_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tms::EBO_QUAD_DATA), tms::EBO_QUAD_DATA, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        _highlighted = false;
    }
    return *this;
}

TMS_Building::TMS_Building(TMS_Building&& oldBuilding) noexcept :
    TMS_Entity(oldBuilding)
{
    /* Move basic building information. */
    _name = std::move(oldBuilding._name);
    _buildTime = oldBuilding._buildTime;
    _buildCost = std::move(oldBuilding._buildCost);
    _fixedProduction = std::move(oldBuilding._fixedProduction);
    _continuousProduction = std::move(oldBuilding._continuousProduction);
    _oneTimeProduction = std::move(oldBuilding._oneTimeProduction);
    _storage = std::move(oldBuilding._storage);
    _highlighted = oldBuilding._highlighted;

    /* Move the building's unique properties. */
    _span = oldBuilding._span;
    _VAO = oldBuilding._VAO;
    oldBuilding._VAO = 0;
    _VBO = oldBuilding._VBO;
    oldBuilding._VBO = 0;
    _EBO = oldBuilding._EBO;
    oldBuilding._EBO = 0;
}

TMS_Building& TMS_Building::operator=(TMS_Building&& oldBuilding) noexcept
{
    if (this != &oldBuilding)
    {
        TMS_Entity::operator=(oldBuilding);

        /* Move basic building information. */
        _name = std::move(oldBuilding._name);
        _buildTime = oldBuilding._buildTime;
        _buildCost = std::move(oldBuilding._buildCost);
        _fixedProduction = std::move(oldBuilding._fixedProduction);
        _continuousProduction = std::move(oldBuilding._continuousProduction);
        _oneTimeProduction = std::move(oldBuilding._oneTimeProduction);
        _storage = std::move(oldBuilding._storage);
        _highlighted = oldBuilding._highlighted;

        /* Move the building's unique properties. */
        _span = oldBuilding._span;
        _VAO = oldBuilding._VAO;
        oldBuilding._VAO = 0;
        _VBO = oldBuilding._VBO;
        oldBuilding._VBO = 0;
        _EBO = oldBuilding._EBO;
        oldBuilding._EBO = 0;
    }
    return *this;
}

tms::Layer TMS_Building::getLayer() const
{
    return tms::default_layer();
}

std::vector<tms::EventType>& TMS_Building::getRelevantEvents()
{
    return _relevantEvents;
}

bool TMS_Building::checkCollision(const int x, const int y) const
{
    if (x >= _span.x && x <= _span.x + _span.w &&
        y >= _span.y && y <= _span.y + _span.h) return true;
    return false;
}

std::optional<TMS_Action> TMS_Building::handleEvent(const SDL_Event& event)
{
    return std::optional<TMS_Action>();
}

void TMS_Building::render()
{
    /* Choose the proper shader. */
    if (_highlighted) _shaders[static_cast<int>(Shader::HIGHLIGHT)]->use();
    else _shaders[static_cast<int>(Shader::PLAIN)]->use();

    /* Render the building. */
    _textures[static_cast<int>(Texture::MAIN)]->bind();
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

const std::string& TMS_Building::getName() const
{
    return _name;
}

void TMS_Building::setName(const std::string& name)
{
    _name = name;
}

void TMS_Building::setSpan(const tms::Rect& span)
{
    /* Update the building's span. */
    _span = span;

    /* Update the building's VBO. */
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    float maxX = static_cast<float>(_span.x + _span.w);
    float maxY = static_cast<float>(_span.y + _span.h);
    float vboData[] =
    {
        static_cast<float>(_span.x), static_cast<float>(_span.y), static_cast<float>(tms::default_layer()), 0.0f, 0.0f, // Top left corner.
        maxX,                        static_cast<float>(_span.y), static_cast<float>(tms::default_layer()), 1.0f, 0.0f, // Top right corner.
        maxX,                        maxY,                        static_cast<float>(tms::default_layer()), 1.0f, 1.0f, // Bottom right corner.
        static_cast<float>(_span.x), maxY,                        static_cast<float>(tms::default_layer()), 0.0f, 1.0f // Bottom left corner.
    };
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vboData), vboData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vboData), vboData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool TMS_Building::setShaders(const std::vector<std::shared_ptr<TMS_Shader>>& shaders)
{
    if (shaders.size() != static_cast<int>(Shader::TOT)) return false;
    _shaders = shaders;
    return true;
}

bool TMS_Building::setBuildTime(const int buildTime)
{
    if (buildTime < 0) return false;
    _buildTime = buildTime;
    return true;
}

void TMS_Building::addBuildCost(const TMS_Item& item)
{
    _buildCost.push_back(item);
}

void TMS_Building::addFixedProduction(const TMS_ItemProduction& production, const std::vector<TMS_ItemCost>& cost)
{
    _fixedProduction.push_back(std::make_pair(production, cost));
}

void TMS_Building::addContinuousProduction(const TMS_ItemProduction& production, const std::vector<TMS_ItemCost>& cost)
{
    _continuousProduction.push_back(std::make_pair(production, cost));
}

void TMS_Building::addOneTimeProduction(const TMS_ItemProduction& production, const std::vector<TMS_ItemCost>& cost)
{
    _oneTimeProduction.push_back(std::make_pair(production, cost));
}

bool TMS_Building::addStorage(const TMS_Item& item, const int maxCapacity)
{
    if (maxCapacity < 0) return false;
    _storage.push_back(std::make_pair(item, maxCapacity));
    return true;
}

void TMS_Building::addTexture(const std::shared_ptr<TMS_Texture>& texture)
{
    _textures.push_back(texture);
}
