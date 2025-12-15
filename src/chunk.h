#pragma once

#include <glm/glm.hpp>

#include "block.h"
#include "mesh.h"
#include "box.h"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 32
#define CHUNK_SIZE_Z 16

struct ChunkVertex
{
    glm::vec3 position;
    glm::vec2 texCoords;
    float light;
};

enum FaceDirection
{
    FACE_EAST = 0,
    FACE_WEST = 1,
    FACE_UP = 2,
    FACE_DOWN = 3,
    FACE_SOUTH = 4,
    FACE_NORTH = 5
};

class Chunk
{
public:
    Chunk(const glm::ivec3& position);
    ~Chunk();

    void generateMesh(Chunk* neighbors[4]);

    void setBlock(int x, int y, int z, BlockType type);
    BlockType getBlock(int x, int y, int z) const;

    inline const glm::ivec3& getPosition() const { return _position; }
    inline Mesh* getOpaqueMesh() const { return _opaqueMesh; }
    inline Mesh* getTransparentMesh() const { return _transparentMesh; }
    inline const Box& getBoundingBox() const { return _boundingBox; }

private:
    bool verifyBlockVisibility(int x, int y, int z, Chunk* neighbor, BlockType currentBlockType);
    bool verifyLocalBlockVisibility(int x, int y, int z, BlockType currentBlockType);
    bool verifyNeighborBlockVisibility(int x, int y, int z, Chunk* neighbor, BlockType currentBlockType);
    void addFaceToMesh(int x, int y, int z, int faceIndex, BlockType block, std::vector<ChunkVertex>& vertices, std::vector<unsigned int>& indices);

private:
    glm::ivec3 _position;
    BlockType _blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
    Mesh* _opaqueMesh;
    Mesh* _transparentMesh;
    Box _boundingBox;
};