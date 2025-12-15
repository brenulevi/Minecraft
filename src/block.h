#pragma once

#include <stdexcept>

#define ATLAS_SIZE 16

enum BlockType
{
    AIR,
    DIRT,
    GRASS,
    STONE,
    WATER
};

struct BlockTexture
{
    int top;
    int bottom;
    int side;
};

bool isTransparent(BlockType block);
bool isRaycastable(BlockType block);

BlockTexture getBlockTexture(BlockType type);

void getAtlasUV(int textureID, float uvs[8]);