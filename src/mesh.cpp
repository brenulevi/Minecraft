#include "mesh.h"

Mesh::Mesh(const BufferLayout &layout)
{
    _vao = new VertexArray();
    _vbo = new VertexBuffer();
    _ibo = new IndexBuffer();

    _vao->addVertexBuffer(*_vbo, layout);
}

Mesh::~Mesh()
{
    delete _ibo;
    delete _vbo;
    delete _vao;
}

void Mesh::setVertices(const void *data, unsigned int size)
{
    _vbo->setData(data, size);
}

void Mesh::setIndices(const unsigned int *data, unsigned int size)
{
    _ibo->setData(data, size / sizeof(unsigned int));
}

void Mesh::bind() const
{
    _vao->bind();
    _ibo->bind();
}

void Mesh::unbind() const
{
    _ibo->unbind();
    _vao->unbind();
}