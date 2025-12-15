#pragma once

#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"

class Mesh
{
public:
    Mesh(const BufferLayout& layout);
    ~Mesh();

    void setVertices(const void* data, unsigned int size);
    void setIndices(const unsigned int* data, unsigned int size);

    void bind() const;
    void unbind() const;

    inline unsigned int getIndexCount() const { return _ibo->getCount(); }

private:
    VertexArray* _vao;
    VertexBuffer* _vbo;
    IndexBuffer* _ibo;
};