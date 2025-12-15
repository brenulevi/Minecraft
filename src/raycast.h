#pragma once

#include <glm/glm.hpp>

#include "block.h"

struct RaycastHit
{
    bool hit;
    glm::vec3 blockPosition;
    glm::vec3 normal;
    float distance;
};

bool raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, RaycastHit& outHit);