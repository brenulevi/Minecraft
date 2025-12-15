#include "raycast.h"

#include "game.h"

bool raycast(const glm::vec3 &origin, const glm::vec3 &direction, float maxDistance, RaycastHit &outHit)
{
    outHit.hit = false;

    // DDA algorithm (Digital Differential Analyzer)
    glm::vec3 pos = origin;
    glm::ivec3 blockPos = glm::ivec3(floor(pos.x), floor(pos.y), floor(pos.z));

    glm::vec3 step = glm::sign(direction);
    glm::vec3 tDelta = glm::abs(1.0f / direction);
    glm::vec3 tMax = (glm::vec3(blockPos) + glm::max(step, glm::vec3(0.0f)) - pos) / direction;

    float distance = 0.0f;

    while (distance < maxDistance)
    {
        BlockType blockType = Game::getInstance().getWorld().getChunkManager().getBlockAt(blockPos);
        if (isRaycastable(blockType))
        {
            outHit.hit = true;
            outHit.blockPosition = glm::vec3(blockPos);
            outHit.distance = distance;
            return true;
        }

        if (tMax.x < tMax.y)
        {
            if (tMax.x < tMax.z)
            {
                blockPos.x += step.x;
                distance = tMax.x;
                tMax.x += tDelta.x;
                outHit.normal = glm::ivec3(-step.x, 0, 0);
            }
            else
            {
                blockPos.z += step.z;
                distance = tMax.z;
                tMax.z += tDelta.z;
                outHit.normal = glm::ivec3(0, 0, -step.z);
            }
        }
        else
        {
            if (tMax.y < tMax.z)
            {
                blockPos.y += step.y;
                distance = tMax.y;
                tMax.y += tDelta.y;
                outHit.normal = glm::ivec3(0, -step.y, 0);
            }
            else
            {
                blockPos.z += step.z;
                distance = tMax.z;
                tMax.z += tDelta.z;
                outHit.normal = glm::ivec3(0, 0, -step.z);
            }
        }
    }

    outHit.hit = false;
    return false;
}