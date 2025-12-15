#include "world.h"

#include <iostream>

World::World(int seed,glm::vec3 &playerPosition)
    : _seed(seed), _playerPosition(playerPosition)
{
    _chunkManager = new ChunkManager(seed);

    _fog.startPoint = RENDER_DISTANCE * CHUNK_SIZE_X * 0.40f;
    _fog.density = 0.0035f;
    _fog.color = glm::vec3(0.5f, 0.5f, 0.5f);

    // Generate initial chunks around player initial position
    glm::ivec3 playerChunkPos = playerPosition;
    playerChunkPos.x = (playerChunkPos.x / CHUNK_SIZE_X) * CHUNK_SIZE_X;
    playerChunkPos.y = (playerChunkPos.y / CHUNK_SIZE_Y) * CHUNK_SIZE_Y;
    playerChunkPos.z = (playerChunkPos.z / CHUNK_SIZE_Z) * CHUNK_SIZE_Z;

    for (int x = -INITIAL_WORLD_RADIUS; x <= INITIAL_WORLD_RADIUS; x++)
    {
        for (int z = -INITIAL_WORLD_RADIUS; z <= INITIAL_WORLD_RADIUS; z++)
        {
            glm::ivec3 chunkPos = playerChunkPos + glm::ivec3(x, 0, z);
            _chunkManager->loadChunkAt(chunkPos);
        }
    }
}

World::~World()
{
    delete _chunkManager;
}

void World::update()
{
    glm::ivec3 playerChunkPos = glm::ivec3(
        std::floor(_playerPosition.x / CHUNK_SIZE_X),
        0,
        std::floor(_playerPosition.z / CHUNK_SIZE_Z)
    );

    // Load new chunks around the player
    for (int x = -RENDER_DISTANCE; x <= RENDER_DISTANCE; x++)
    {
        for (int z = -RENDER_DISTANCE; z <= RENDER_DISTANCE; z++)
        {
            float distance = std::sqrt(float(x * x + z * z));
            if (distance > RENDER_DISTANCE)
                continue;

            glm::ivec3 chunkPos = playerChunkPos + glm::ivec3(x, 0, z);
            _chunkManager->loadChunkAt(chunkPos);
        }
    }

    // Unload distant chunks
    std::vector<glm::ivec3> chunksToUnload;
    for (const auto& pair : _chunkManager->getAllChunks())
    {
        glm::ivec3 chunkPos = pair.first;
        glm::ivec3 diff = chunkPos - playerChunkPos;
        float distance = std::sqrt(float(diff.x * diff.x + diff.z * diff.z));
        if (distance > RENDER_DISTANCE + 2)
        {
            chunksToUnload.push_back(chunkPos);
        }
    }

    for (const auto& chunkPos : chunksToUnload)
    {
        _chunkManager->unloadChunkAt(chunkPos);
    }

    _chunkManager->generateMeshForPendingChunks();
}

void World::breakBlockAt(const glm::ivec3 &position)
{
    _chunkManager->setBlockAt(position, AIR);
}

void World::placeBlockAt(const glm::ivec3 &position, BlockType type)
{
    _chunkManager->setBlockAt(position, type);
}

void World::onResize(int width, int height)
{
    
}
