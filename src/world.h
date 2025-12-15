#pragma once

#include "chunkmanager.h"
#include "fog.h"

#define INITIAL_WORLD_RADIUS 5
#define RENDER_DISTANCE 8

class World
{
public:
    World(int seed,glm::vec3& playerPosition);
    ~World();

    void update();

    void breakBlockAt(const glm::ivec3& position);
    void placeBlockAt(const glm::ivec3& position, BlockType type);

    void onResize(int width, int height);

    inline ChunkManager& getChunkManager() const { return *_chunkManager; }
    inline const Fog& getFog() const { return _fog; }

private:
    int _seed;

    ChunkManager* _chunkManager;

    Fog _fog;
    
    glm::vec3& _playerPosition;
};