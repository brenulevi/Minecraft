#pragma once

#include <glad/glad.h>

class IndexBuffer
{
public:
    IndexBuffer();
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void setData(const unsigned int* data, unsigned int count);

    void bind() const;
    void unbind() const;

    inline unsigned int getCount() const { return _count; }

private:
    unsigned int _id;
    unsigned int _count;
};