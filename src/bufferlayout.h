#pragma once

#include <glad/glad.h>
#include <vector>
#include <stdexcept>

struct BufferLayoutElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        }
        return 0;
    }
};

class BufferLayout
{
public:
    BufferLayout() : _stride(0) {}

    template <typename T>
    void push(unsigned int count);

    inline const std::vector<BufferLayoutElement> &getElements() const { return _elements; }
    inline unsigned int getStride() const { return _stride; }

private:
    std::vector<BufferLayoutElement> _elements;
    unsigned int _stride;
};

template <typename T>
inline void BufferLayout::push(unsigned int count)
{
    throw std::runtime_error("Unsupported type");
}

template <>
inline void BufferLayout::push<float>(unsigned int count)
{
    _elements.push_back({GL_FLOAT, count, GL_FALSE});
    _stride += count * BufferLayoutElement::getSizeOfType(GL_FLOAT);
}

template <>
inline void BufferLayout::push<unsigned int>(unsigned int count)
{
    _elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    _stride += count * BufferLayoutElement::getSizeOfType(GL_UNSIGNED_INT);
}

template <>
inline void BufferLayout::push<unsigned char>(unsigned int count)
{
    _elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    _stride += count * BufferLayoutElement::getSizeOfType(GL_UNSIGNED_BYTE);
}
