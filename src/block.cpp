#include "block.h"

bool isTransparent(BlockType block)
{
    return block == AIR || block == WATER;
}

bool isRaycastable(BlockType block)
{
    return block != AIR && block != WATER;
}

BlockTexture getBlockTexture(BlockType type)
{
    switch (type)
    {
    case GRASS:
        return {0, 1, 2};
    case DIRT:
        return {1, 1, 1};
    case STONE:
        return {3, 3, 3};
    case WATER:
        return {4, 4, 4};
    default:
        throw std::runtime_error("Unknown BlockType");
    }
}

void getAtlasUV(int textureID, float uvs[8])
{
    int atlasX = textureID % ATLAS_SIZE;
    int atlasY = textureID / ATLAS_SIZE;

    float uvSize = 1.0f / static_cast<float>(ATLAS_SIZE);

    float uMin = atlasX * uvSize;
    float uMax = (atlasX + 1) * uvSize;
    float vMin = atlasY * uvSize;
    float vMax = (atlasY + 1) * uvSize;

    uvs[0] = uMin; uvs[1] = vMin; // Bottom-left
    uvs[2] = uMax; uvs[3] = vMin; // Bottom-right
    uvs[4] = uMax; uvs[5] = vMax; // Top-right
    uvs[6] = uMin; uvs[7] = vMax; // Top-left
}