#pragma once

#include <glm/glm.hpp>

#include "box.h"

class Frustum
{
public:
    Frustum();
    ~Frustum();

    void update(const glm::mat4& projectionViewMatrix);

    bool isBoxVisible(const Box& box) const;

private:
    glm::vec4 _planes[6];
};