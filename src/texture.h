#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include <stdexcept>

class Texture
{
public:
    Texture(const char* filePath);
    ~Texture();

    void setFiltering(GLenum minFilter, GLenum magFilter);
    void setWrapMode(GLenum wrapS, GLenum wrapT);

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getWidth() const { return _width; }
    inline int getHeight() const { return _height; }

private:
    unsigned int _id;
    int _width;
    int _height;
    int _channels;
};