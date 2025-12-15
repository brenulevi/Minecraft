#pragma once

#include <glad/glad.h>

#include "bufferlayout.h"
#include "vertexbuffer.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addVertexBuffer(const VertexBuffer& vb, const BufferLayout& layout);

private:
    unsigned int _id;
};