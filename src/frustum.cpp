#include "frustum.h"

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::update(const glm::mat4& projectionViewMatrix)
{
    // Extract planes from the combined projection-view matrix
    // Left plane
    _planes[0] = glm::vec4(
        projectionViewMatrix[0][3] + projectionViewMatrix[0][0],
        projectionViewMatrix[1][3] + projectionViewMatrix[1][0],
        projectionViewMatrix[2][3] + projectionViewMatrix[2][0],
        projectionViewMatrix[3][3] + projectionViewMatrix[3][0]
    );
    // Right plane
    _planes[1] = glm::vec4(
        projectionViewMatrix[0][3] - projectionViewMatrix[0][0],
        projectionViewMatrix[1][3] - projectionViewMatrix[1][0],
        projectionViewMatrix[2][3] - projectionViewMatrix[2][0],
        projectionViewMatrix[3][3] - projectionViewMatrix[3][0]
    );
    // Bottom plane
    _planes[2] = glm::vec4(
        projectionViewMatrix[0][3] + projectionViewMatrix[0][1],
        projectionViewMatrix[1][3] + projectionViewMatrix[1][1],
        projectionViewMatrix[2][3] + projectionViewMatrix[2][1],
        projectionViewMatrix[3][3] + projectionViewMatrix[3][1]
    );
    // Top plane
    _planes[3] = glm::vec4(
        projectionViewMatrix[0][3] - projectionViewMatrix[0][1],
        projectionViewMatrix[1][3] - projectionViewMatrix[1][1],
        projectionViewMatrix[2][3] - projectionViewMatrix[2][1],
        projectionViewMatrix[3][3] - projectionViewMatrix[3][1]
    );
    // Near plane
    _planes[4] = glm::vec4(
        projectionViewMatrix[0][3] + projectionViewMatrix[0][2],
        projectionViewMatrix[1][3] + projectionViewMatrix[1][2],
        projectionViewMatrix[2][3] + projectionViewMatrix[2][2],
        projectionViewMatrix[3][3] + projectionViewMatrix[3][2]
    );
    // Far plane
    _planes[5] = glm::vec4(
        projectionViewMatrix[0][3] - projectionViewMatrix[0][2],
        projectionViewMatrix[1][3] - projectionViewMatrix[1][2],
        projectionViewMatrix[2][3] - projectionViewMatrix[2][2],
        projectionViewMatrix[3][3] - projectionViewMatrix[3][2]
    );

    // Normalize the planes
    for (int i = 0; i < 6; ++i)
    {
        float length = glm::length(glm::vec3(_planes[i]));
        _planes[i] /= length;
    }

}

bool Frustum::isBoxVisible(const Box &box) const
{
    for (int i = 0; i < 6; ++i)
    {
        const glm::vec4& plane = _planes[i];

        glm::vec3 positiveVertex = box.min;
        if (plane.x >= 0) positiveVertex.x = box.max.x;
        if (plane.y >= 0) positiveVertex.y = box.max.y;
        if (plane.z >= 0) positiveVertex.z = box.max.z;

        // If the positive vertex is outside the plane, the box is not visible
        if (glm::dot(glm::vec3(plane), positiveVertex) + plane.w < 0)
        {
            return false;
        }
    }
    return true;
}
