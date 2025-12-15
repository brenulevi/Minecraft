#include "renderer.h"

Renderer::Renderer(int width, int height)
    : _width(width), _height(height)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");

    _chunkShader = new Shader("assets/shaders/chunk.vert", "assets/shaders/chunk.frag");
    _uiShader = new Shader("assets/shaders/ui.vert", "assets/shaders/ui.frag");
    _outlineShader = new Shader("assets/shaders/outline.vert", "assets/shaders/outline.frag");

    _textureAtlas = new Texture("assets/textures/atlas.png");
    generateMeshes();

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _viewMatrix = glm::mat4(1.0f);
    _perspectiveMatrix = glm::mat4(1.0f);
    _orthographicMatrix = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);

    _uiShader->use();
    _uiShader->setUniformMat4f("orthographic", glm::value_ptr(_orthographicMatrix));
}

Renderer::~Renderer()
{
    delete _chunkShader;
    delete _uiShader;

    delete _textureAtlas;

    delete _outlineMesh;
    delete _quadMesh;
}

void Renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setCamera(const Transform &t, const Camera &c)
{
    _viewMatrix = glm::lookAt(t.position, t.position + t.forward, t.up);
    _perspectiveMatrix = c.getProjectionMatrix();

    glm::mat4 vpMatrix = _perspectiveMatrix * _viewMatrix;
    _frustum.update(vpMatrix);

    _chunkShader->use();
    _chunkShader->setUniformMat4f("projection", glm::value_ptr(_perspectiveMatrix));
    _chunkShader->setUniformMat4f("view", glm::value_ptr(_viewMatrix));

    _outlineShader->use();
    _outlineShader->setUniformMat4f("projection", glm::value_ptr(_perspectiveMatrix));
    _outlineShader->setUniformMat4f("view", glm::value_ptr(_viewMatrix));
}

void Renderer::renderWorld(const World &world)
{
    // Render chunks
    _chunkShader->use();

    _textureAtlas->bind(0);
    _chunkShader->setUniform1i("atlas", 0); // Texture unit 0
    _chunkShader->setUniform3f("ambientColor", 1.0f, 1.0f, 1.0f);
   
    const Fog& fog = world.getFog();
    _chunkShader->setUniform1f("fogStart", fog.startPoint);
    _chunkShader->setUniform3f("fogColor", fog.color.r, fog.color.g, fog.color.b);
    _chunkShader->setUniform1f("fogDensity", fog.density);

    for (auto &pair : world.getChunkManager().getAllChunks())
    {
        Chunk *chunk = pair.second;

        Mesh *mesh = chunk->getMesh();

        if (!mesh || !_frustum.isBoxVisible(chunk->getBoundingBox()))
            continue;

        glm::vec3 chunkPos = chunk->getPosition();
        glm::vec3 chunkWorldPos = glm::vec3(chunkPos.x, chunkPos.y, chunkPos.z) * glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, chunkWorldPos);
        _chunkShader->setUniformMat4f("model", glm::value_ptr(model));

        mesh->bind();
        glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
    }
}

void Renderer::renderUI(const UIManager &uiManager)
{
    // Render crosshair
    glDisable(GL_DEPTH_TEST);

    _uiShader->use();

    Crosshair *crosshair = uiManager.crosshair;

    glm::vec3 centerPos = glm::vec3(_width / 2.0f, _height / 2.0f, 0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, centerPos);
    model = glm::rotate(model, glm::radians(crosshair->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(crosshair->size, crosshair->size, 1.0f));
    _uiShader->setUniformMat4f("model", glm::value_ptr(model));

    crosshair->texture->bind(0);
    _uiShader->setUniform1i("uiTexture", 0); // Texture unit 0

    _quadMesh->bind();
    glDrawElements(GL_TRIANGLES, _quadMesh->getIndexCount(), GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
}

void Renderer::renderOutline(RaycastHit &hitInfo, const glm::vec3 &color)
{
    if (!hitInfo.hit)
        return;

    glLineWidth(2.5f);

    glDepthMask(GL_FALSE);

    _outlineShader->use();

    _outlineShader->setUniform3f("outlineColor", color.r, color.g, color.b);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, hitInfo.blockPosition + glm::vec3(0.5f));
    _outlineShader->setUniformMat4f("model", glm::value_ptr(model));

    _outlineMesh->bind();

    glDrawElements(GL_LINES, _outlineMesh->getIndexCount(), GL_UNSIGNED_INT, 0);

    glDepthMask(GL_TRUE);
}

void Renderer::onResize(int width, int height)
{
    _width = width;
    _height = height;
    glViewport(0, 0, width, height);
    _orthographicMatrix = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
    _uiShader->use();
    _uiShader->setUniformMat4f("orthographic", glm::value_ptr(_orthographicMatrix));
}

void Renderer::generateMeshes()
{
    float quadVertices[] = {
        // Positions        // Texture Coords
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // Bottom-right
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // Top-right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f   // Top-left
    };

    unsigned int quadIndices[] = {
        0, 1, 2,
        2, 3, 0};

    BufferLayout layout;
    layout.push<float>(3); // Position
    layout.push<float>(2); // Texture Coords

    _quadMesh = new Mesh(layout);
    _quadMesh->setVertices(quadVertices, sizeof(quadVertices));
    _quadMesh->setIndices(quadIndices, sizeof(quadIndices));

    float outlineVertices[] = {
        // bottom face
        -0.5f, -0.5f, -0.5f, // 0
        0.5f, -0.5f, -0.5f,  // 1
        0.5f, 0.5f, -0.5f,   // 2
        -0.5f, 0.5f, -0.5f,  // 3
        // top face
        -0.5f, -0.5f, 0.5f, // 4
        0.5f, -0.5f, 0.5f,  // 5
        0.5f, 0.5f, 0.5f,   // 6
        -0.5f, 0.5f, 0.5f   // 7
    };

    unsigned int outlineIndices[] = {
        // bottom
        0, 1, 1, 2, 2, 3, 3, 0,
        // top
        4, 5, 5, 6, 6, 7, 7, 4,
        // verticals
        0, 4, 1, 5, 2, 6, 3, 7};

    BufferLayout outlineLayout;
    outlineLayout.push<float>(3); // Position

    _outlineMesh = new Mesh(outlineLayout);
    _outlineMesh->setVertices(outlineVertices, sizeof(outlineVertices));
    _outlineMesh->setIndices(outlineIndices, sizeof(outlineIndices));
}
