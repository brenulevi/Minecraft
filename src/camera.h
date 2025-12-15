#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera
{
    float fov;
    float aspectRatio;
    float nearClip;
    float farClip;

    Camera(float fov,
           float aspectRatio,
           float nearClip,
           float farClip)
        : fov(fov), aspectRatio(aspectRatio), nearClip(nearClip), farClip(farClip) {}

    glm::mat4 getProjectionMatrix() const
    {
        return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
    }
};