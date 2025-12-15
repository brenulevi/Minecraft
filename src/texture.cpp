#include "texture.h"

Texture::Texture(const char *filePath)
{
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(filePath, &_width, &_height, &_channels, 4);
    if(!data)
        throw std::runtime_error("Failed to load texture image: " + std::string(filePath));

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    setFiltering(GL_NEAREST, GL_NEAREST);
    setWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &_id);
}

void Texture::setFiltering(GLenum minFilter, GLenum magFilter)
{
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::setWrapMode(GLenum wrapS, GLenum wrapT)
{
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
}

void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
