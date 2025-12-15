#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;

    Transform(const glm::vec3& pos, 
              const glm::vec3& rot, 
              const glm::vec3& scl)
        : position(pos), rotation(rot), scale(scl)
    {
        updateVectors();
    }

    void updateVectors()
    {
        // Calculate forward vector
        forward.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
        forward.y = sin(glm::radians(rotation.x));
        forward.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
        forward = glm::normalize(forward);

        // Calculate right and up vectors
        right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        up = glm::normalize(glm::cross(right, forward));
    }

    glm::mat4 getModelMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }
};