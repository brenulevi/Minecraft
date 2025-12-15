#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "frustum.h"

#include "transform.h"
#include "camera.h"

#include "world.h"
#include "uimanager.h"
#include "raycast.h"

class Renderer
{
public:
    Renderer(int width, int height);
    ~Renderer();

    void clear();
    void setCamera(const Transform& t, const Camera& c);
    void renderWorld(const World& world);
    void renderUI(const UIManager& uiManager);
    void renderOutline(RaycastHit& hitInfo, const glm::vec3& color);

    void onResize(int width, int height);

private:
    void generateMeshes();

private:
    Shader* _chunkShader;
    Shader* _uiShader;
    Shader* _outlineShader;

    Texture* _textureAtlas;

    Mesh* _outlineMesh;
    Mesh* _quadMesh;

    Frustum _frustum;

    glm::mat4 _viewMatrix;
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _orthographicMatrix;

    int _width;
    int _height;
};