#pragma once

#include <glad/glad.h>

class VertexBuffer
{
public:
    VertexBuffer();
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void setData(const void* data, unsigned int size);

    void bind() const;
    void unbind() const;

private:
    unsigned int _id;
};