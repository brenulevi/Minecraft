#pragma once

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <glm/glm.hpp>

#include "chunk.h"
#include "noises/opensimplexnoise.h"

#define CHUNK_GENERATION_PER_FRAME 4
#define NOISE_FREQUENCY 0.01f

namespace std
{
    template<>
    struct hash<glm::ivec3>
    {
        size_t operator()(const glm::ivec3& v) const
        {
            return ((std::hash<int>()(v.x) ^
                    (std::hash<int>()(v.y) << 1)) >> 1) ^
                    (std::hash<int>()(v.z) << 1);
        }
    };
}

class ChunkManager
{
public:
    ChunkManager(int seed);
    ~ChunkManager();

    void generateMeshForPendingChunks();

    void loadChunkAt(const glm::ivec3& position);
    void unloadChunkAt(const glm::ivec3& position);

    void setBlockAt(const glm::ivec3& globalPosition, BlockType type);

    Chunk* getChunkAt(const glm::ivec3& position);
    BlockType getBlockAt(const glm::ivec3& globalPosition);

    inline const std::unordered_map<glm::ivec3, Chunk*>& getAllChunks() const { return _chunks; }

private:
    std::unordered_map<glm::ivec3, Chunk*> _chunks;
    std::queue<Chunk*> _chunksToGenerateMesh;
    std::unordered_set<Chunk*> _queuedChunks;

    OpenSimplexNoise* _noise;
};