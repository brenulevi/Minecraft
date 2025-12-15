#include "chunkManager.h"

ChunkManager::ChunkManager(int seed)
{
    _noise = new OpenSimplexNoise(seed, NOISE_FREQUENCY);
}

ChunkManager::~ChunkManager()
{
    for(auto& pair : _chunks)
        delete pair.second;

    delete _noise;
}

void ChunkManager::generateMeshForPendingChunks()
{
    int chunksProcessed = 0;
    while(!_chunksToGenerateMesh.empty() && chunksProcessed < CHUNK_GENERATION_PER_FRAME)
    {
        Chunk* chunk = _chunksToGenerateMesh.front();
        _chunksToGenerateMesh.pop();

        // Libera este chunk para poder ser enfileirado novamente no futuro
        _queuedChunks.erase(chunk);

        Chunk* neighbors[4] = {nullptr, nullptr, nullptr, nullptr};

        glm::ivec3 pos = chunk->getPosition();
        neighbors[0] = getChunkAt(pos + glm::ivec3(1, 0, 0)); // +X
        neighbors[1] = getChunkAt(pos + glm::ivec3(-1, 0, 0)); // -X
        neighbors[2] = getChunkAt(pos + glm::ivec3(0, 0, 1)); // +Z
        neighbors[3] = getChunkAt(pos + glm::ivec3(0, 0, -1)); // -Z

        chunk->generateMesh(neighbors);
        chunksProcessed++;
    }
}

void ChunkManager::loadChunkAt(const glm::ivec3 &position)
{
    if(getChunkAt(position))
        return; // Chunk already loaded

    Chunk* newChunk = new Chunk(position);

    for(int x = 0; x < CHUNK_SIZE_X; x++)
    {
        for(int z = 0; z < CHUNK_SIZE_Z; z++)
        {
            // Generate height using noise
            int globalX = position.x * CHUNK_SIZE_X + x;
            int globalZ = position.z * CHUNK_SIZE_Z + z;
            int height = (_noise->noise2D(globalX, globalZ) + 1.0) / 2.0 * (CHUNK_SIZE_Y - 1);
            
            for(int y = 0; y < CHUNK_SIZE_Y; y++)
            {
                if(y <= height)
                    newChunk->setBlock(x, y, z, DIRT);
                else
                    newChunk->setBlock(x, y, z, AIR);
            }
        }
    }

    _chunks[position] = newChunk;

    // Enfileira apenas se ainda não estiver na fila
    if(_queuedChunks.insert(newChunk).second)
        _chunksToGenerateMesh.push(newChunk);

    // Mark neighboring chunks for mesh regeneration
    glm::ivec3 neighborPositions[4] = {
        position + glm::ivec3(1, 0, 0),
        position + glm::ivec3(-1, 0, 0),
        position + glm::ivec3(0, 0, 1),
        position + glm::ivec3(0, 0, -1)
    };

    for (const auto& neighborPos : neighborPositions)
    {
        Chunk* neighborChunk = getChunkAt(neighborPos);
        if (neighborChunk)
        {
            if(_queuedChunks.insert(neighborChunk).second)
                _chunksToGenerateMesh.push(neighborChunk);
        }
    }
}

void ChunkManager::unloadChunkAt(const glm::ivec3 &position)
{
    auto it = _chunks.find(position);
    if(it != _chunks.end())
    {
        Chunk* chunkToRemove = it->second;

        // Remove este chunk da fila de geração de mesh, se estiver lá
        std::queue<Chunk*> tempQueue;
        while(!_chunksToGenerateMesh.empty())
        {
            Chunk* current = _chunksToGenerateMesh.front();
            _chunksToGenerateMesh.pop();

            if(current != chunkToRemove)
                tempQueue.push(current);
        }
        _chunksToGenerateMesh = std::move(tempQueue);

        // Remove também do set de controle de enfileirados
        _queuedChunks.erase(chunkToRemove);

        // Finalmente, deleta e remove do mapa
        delete chunkToRemove;
        _chunks.erase(it);
    }
}

void ChunkManager::setBlockAt(const glm::ivec3 &globalPosition, BlockType type)
{
    glm::ivec3 chunkPos(
        globalPosition.x >= 0 ? globalPosition.x / CHUNK_SIZE_X : (globalPosition.x - CHUNK_SIZE_X + 1) / CHUNK_SIZE_X,
        0,
        globalPosition.z >= 0 ? globalPosition.z / CHUNK_SIZE_Z : (globalPosition.z - CHUNK_SIZE_Z + 1) / CHUNK_SIZE_Z
    );

    Chunk* chunk = getChunkAt(chunkPos);
    if(!chunk)
        return;

    glm::ivec3 localPos(
        globalPosition.x - chunkPos.x * CHUNK_SIZE_X,
        globalPosition.y,
        globalPosition.z - chunkPos.z * CHUNK_SIZE_Z
    );

    chunk->setBlock(localPos.x, localPos.y, localPos.z, type);

    // Enfileira o chunk para regeneração de mesh, se ainda não estiver na fila
    if(_queuedChunks.insert(chunk).second)
        _chunksToGenerateMesh.push(chunk);

    // Verifica se o bloco está na borda do chunk e enfileira chunks vizinhos se necessário
    if(localPos.x == 0)
    {
        Chunk* neighbor = getChunkAt(chunkPos + glm::ivec3(-1, 0, 0));
        if(neighbor && _queuedChunks.insert(neighbor).second)
            _chunksToGenerateMesh.push(neighbor);
    }
    else if(localPos.x == CHUNK_SIZE_X - 1)
    {
        Chunk* neighbor = getChunkAt(chunkPos + glm::ivec3(1, 0, 0));
        if(neighbor && _queuedChunks.insert(neighbor).second)
            _chunksToGenerateMesh.push(neighbor);
    }

    if(localPos.z == 0)
    {
        Chunk* neighbor = getChunkAt(chunkPos + glm::ivec3(0, 0, -1));
        if(neighbor && _queuedChunks.insert(neighbor).second)
            _chunksToGenerateMesh.push(neighbor);
    }
    else if(localPos.z == CHUNK_SIZE_Z - 1)
    {
        Chunk* neighbor = getChunkAt(chunkPos + glm::ivec3(0, 0, 1));
        if(neighbor && _queuedChunks.insert(neighbor).second)
            _chunksToGenerateMesh.push(neighbor);
    }
}

Chunk *ChunkManager::getChunkAt(const glm::ivec3 &position)
{
    auto it = _chunks.find(position);
    if(it != _chunks.end())
        return it->second;
    return nullptr;
}

BlockType ChunkManager::getBlockAt(const glm::ivec3 &globalPosition)
{
    glm::ivec3 chunkPos(
        globalPosition.x >= 0 ? globalPosition.x / CHUNK_SIZE_X : (globalPosition.x - CHUNK_SIZE_X + 1) / CHUNK_SIZE_X,
        0,
        globalPosition.z >= 0 ? globalPosition.z / CHUNK_SIZE_Z : (globalPosition.z - CHUNK_SIZE_Z + 1) / CHUNK_SIZE_Z
    );

    Chunk* chunk = getChunkAt(chunkPos);
    if(!chunk)
        return AIR;

    glm::ivec3 localPos(
        globalPosition.x - chunkPos.x * CHUNK_SIZE_X,
        globalPosition.y,
        globalPosition.z - chunkPos.z * CHUNK_SIZE_Z
    );

    return chunk->getBlock(localPos.x, localPos.y, localPos.z);
}
