#include "chunk.h"

Chunk::Chunk(const glm::ivec3 &position)
    : _position(position), _opaqueMesh(nullptr), _transparentMesh(nullptr)
{
    _boundingBox.min = glm::vec3(position) * glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
    _boundingBox.max = glm::vec3(position + glm::ivec3(1, 1, 1)) * glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
}

Chunk::~Chunk()
{
    if (_opaqueMesh)
        delete _opaqueMesh;
    if (_transparentMesh)
        delete _transparentMesh;
}

void Chunk::generateMesh(Chunk *neighbors[4])
{
    if (_opaqueMesh)
        delete _opaqueMesh;
    if (_transparentMesh)
        delete _transparentMesh;

    BufferLayout layout;
    layout.push<float>(3); // Position
    layout.push<float>(2); // Texture Coords
    layout.push<float>(1); // Light

    _opaqueMesh = new Mesh(layout);
    _transparentMesh = new Mesh(layout);

    std::vector<ChunkVertex> opaqueVertices;
    std::vector<unsigned int> opaqueIndices;
    std::vector<ChunkVertex> transparentVertices;
    std::vector<unsigned int> transparentIndices;

    for (int x = 0; x < CHUNK_SIZE_X; x++)
    {
        for (int y = 0; y < CHUNK_SIZE_Y; y++)
        {
            for (int z = 0; z < CHUNK_SIZE_Z; z++)
            {
                BlockType block = getBlock(x, y, z);

                if (block == AIR)
                    continue;

                bool isFaceVisible[6] = {false, false, false, false, false, false};
                isFaceVisible[FACE_EAST] = verifyBlockVisibility(x + 1, y, z, neighbors[0], block);  // +X
                isFaceVisible[FACE_WEST] = verifyBlockVisibility(x - 1, y, z, neighbors[1], block);  // -X
                isFaceVisible[FACE_UP] = verifyBlockVisibility(x, y + 1, z, nullptr, block);         // +Y
                isFaceVisible[FACE_DOWN] = verifyBlockVisibility(x, y - 1, z, nullptr, block);       // -Y
                isFaceVisible[FACE_SOUTH] = verifyBlockVisibility(x, y, z + 1, neighbors[2], block); // +Z
                isFaceVisible[FACE_NORTH] = verifyBlockVisibility(x, y, z - 1, neighbors[3], block); // -Z

                for (int i = 0; i < 6; i++)
                {
                    if (isFaceVisible[i])
                    {
                        if (isTransparent(block))
                            addFaceToMesh(x, y, z, i, block, transparentVertices, transparentIndices);
                        else
                            addFaceToMesh(x, y, z, i, block, opaqueVertices, opaqueIndices);
                    }
                }
            }
        }
    }

    _opaqueMesh->setVertices(opaqueVertices.data(), opaqueVertices.size() * sizeof(ChunkVertex));
    _opaqueMesh->setIndices(opaqueIndices.data(), opaqueIndices.size() * sizeof(unsigned int));
    _transparentMesh->setVertices(transparentVertices.data(), transparentVertices.size() * sizeof(ChunkVertex));
    _transparentMesh->setIndices(transparentIndices.data(), transparentIndices.size() * sizeof(unsigned int));
}

void Chunk::setBlock(int x, int y, int z, BlockType type)
{
    if (x < 0 || x >= CHUNK_SIZE_X ||
        y < 0 || y >= CHUNK_SIZE_Y ||
        z < 0 || z >= CHUNK_SIZE_Z)
        return;

    _blocks[x][y][z] = type;
}

BlockType Chunk::getBlock(int x, int y, int z) const
{
    if (x < 0 || x >= CHUNK_SIZE_X ||
        y < 0 || y >= CHUNK_SIZE_Y ||
        z < 0 || z >= CHUNK_SIZE_Z)
        return AIR;

    return _blocks[x][y][z];
}

bool Chunk::verifyBlockVisibility(int x, int y, int z, Chunk *neighbor, BlockType currentBlockType)
{
    if (y < 0 || y >= CHUNK_SIZE_Y)
    {
        // Out of vertical bounds, consider visible
        return true;
    }

    if (x < 0 || x >= CHUNK_SIZE_X ||
        z < 0 || z >= CHUNK_SIZE_Z)
    {
        glm::ivec3 blockGlobalPosition(
            _position.x * CHUNK_SIZE_X + x,
            y,
            _position.z * CHUNK_SIZE_Z + z);
        return verifyNeighborBlockVisibility(x, y, z, neighbor, currentBlockType);
    }

    return verifyLocalBlockVisibility(x, y, z, currentBlockType);
}

bool Chunk::verifyLocalBlockVisibility(int x, int y, int z, BlockType currentBlockType)
{
    if (x < 0 || x >= CHUNK_SIZE_X ||
        y < 0 || y >= CHUNK_SIZE_Y ||
        z < 0 || z >= CHUNK_SIZE_Z)
        return true;

    BlockType neighborBlock = getBlock(x, y, z);
    if (currentBlockType == WATER)
        return neighborBlock == AIR; // água só abre face contra ar
    return isTransparent(neighborBlock); // blocos opacos abrem contra transparência (ar/água)
}

bool Chunk::verifyNeighborBlockVisibility(int x, int y, int z, Chunk *neighbor, BlockType currentBlockType)
{
    if (!neighbor)
        return true;

    // Convert local coordinates to neighbor chunk coordinates
    if (x < 0)
        x += CHUNK_SIZE_X;
    else if (x >= CHUNK_SIZE_X)
        x -= CHUNK_SIZE_X;
    if (z < 0)
        z += CHUNK_SIZE_Z;
    else if (z >= CHUNK_SIZE_Z)
        z -= CHUNK_SIZE_Z;

    BlockType neighborBlock = neighbor->getBlock(x, y, z);
    if (currentBlockType == WATER)
        return neighborBlock == AIR;
    return isTransparent(neighborBlock);
}

void Chunk::addFaceToMesh(int x, int y, int z, int faceIndex, BlockType block, std::vector<ChunkVertex> &vertices, std::vector<unsigned int> &indices)
{
    // Get UV coordinates based on block type
    BlockTexture textures = getBlockTexture(block);

    int textureID;
    float light = 1.0f; // Full brightness for now
    switch (faceIndex)
    {
    case FACE_UP:
    {
        textureID = textures.top;
        light = 1.0f;
        break;
    }
    case FACE_DOWN:
    {
        textureID = textures.bottom;
        light = 0.2f;
        break;
    }
    default:
    {
        textureID = textures.side;
        light = 0.4f;
        break;
    }
    }

    float uvCoords[8];
    getAtlasUV(textureID, uvCoords);

    if (faceIndex == FACE_EAST) // +X
    {
        // Origem local: (x+1, y, z+1), s = -Z, t = +Y
        vertices.push_back({glm::vec3(x + 1, y, z), glm::vec2(uvCoords[0], uvCoords[1]), light});         // bottom-left
        vertices.push_back({glm::vec3(x + 1, y + 1, z), glm::vec2(uvCoords[6], uvCoords[7]), light});     // bottom-right
        vertices.push_back({glm::vec3(x + 1, y + 1, z + 1), glm::vec2(uvCoords[4], uvCoords[5]), light}); // top-right
        vertices.push_back({glm::vec3(x + 1, y, z + 1), glm::vec2(uvCoords[2], uvCoords[3]), light});     // top-left
    }
    else if (faceIndex == FACE_WEST) // -X
    {
        // Origem local: (x, y, z), s = +Z, t = +Y
        vertices.push_back({glm::vec3(x, y, z + 1), glm::vec2(uvCoords[2], uvCoords[3]), light});     // bottom-left
        vertices.push_back({glm::vec3(x, y + 1, z + 1), glm::vec2(uvCoords[4], uvCoords[5]), light}); // bottom-right
        vertices.push_back({glm::vec3(x, y + 1, z), glm::vec2(uvCoords[6], uvCoords[7]), light});     // top-right
        vertices.push_back({glm::vec3(x, y, z), glm::vec2(uvCoords[0], uvCoords[1]), light});         // top-left
    }
    else if (faceIndex == FACE_UP) // +Y
    {
        // Origem local: (x, y+1, z+1), s = +X, t = -Z
        vertices.push_back({glm::vec3(x, y + 1, z), glm::vec2(uvCoords[0], uvCoords[1]), light});         // bottom-left
        vertices.push_back({glm::vec3(x, y + 1, z + 1), glm::vec2(uvCoords[6], uvCoords[7]), light});     // bottom-right
        vertices.push_back({glm::vec3(x + 1, y + 1, z + 1), glm::vec2(uvCoords[4], uvCoords[5]), light}); // top-right
        vertices.push_back({glm::vec3(x + 1, y + 1, z), glm::vec2(uvCoords[2], uvCoords[3]), light});     // top-left
    }
    else if (faceIndex == FACE_DOWN) // -Y
    {
        // Origem local: (x, y, z), s = +X, t = +Z
        vertices.push_back({glm::vec3(x, y, z), glm::vec2(uvCoords[0], uvCoords[1]), light});         // bottom-left
        vertices.push_back({glm::vec3(x + 1, y, z), glm::vec2(uvCoords[2], uvCoords[3]), light});     // bottom-right
        vertices.push_back({glm::vec3(x + 1, y, z + 1), glm::vec2(uvCoords[4], uvCoords[5]), light}); // top-right
        vertices.push_back({glm::vec3(x, y, z + 1), glm::vec2(uvCoords[6], uvCoords[7]), light});     // top-left
    }
    else if (faceIndex == FACE_SOUTH) // +Z
    {
        // Origem local: (x, y, z+1), s = +X, t = +Y
        vertices.push_back({glm::vec3(x + 1, y, z + 1), glm::vec2(uvCoords[2], uvCoords[3]), light});     // bottom-left
        vertices.push_back({glm::vec3(x + 1, y + 1, z + 1), glm::vec2(uvCoords[4], uvCoords[5]), light}); // bottom-right
        vertices.push_back({glm::vec3(x, y + 1, z + 1), glm::vec2(uvCoords[6], uvCoords[7]), light});     // top-right
        vertices.push_back({glm::vec3(x, y, z + 1), glm::vec2(uvCoords[0], uvCoords[1]), light});         // top-left
    }
    else if (faceIndex == FACE_NORTH) // -Z
    {
        // Origem local: (x+1, y, z), s = -X, t = +Y
        vertices.push_back({glm::vec3(x, y, z), glm::vec2(uvCoords[0], uvCoords[1]), light});         // bottom-left
        vertices.push_back({glm::vec3(x, y + 1, z), glm::vec2(uvCoords[6], uvCoords[7]), light});     // bottom-right
        vertices.push_back({glm::vec3(x + 1, y + 1, z), glm::vec2(uvCoords[4], uvCoords[5]), light}); // top-right
        vertices.push_back({glm::vec3(x + 1, y, z), glm::vec2(uvCoords[2], uvCoords[3]), light});     // top-left
    }

    unsigned int startIndex = vertices.size() - 4;
    indices.push_back(startIndex + 0);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex + 3);
}
