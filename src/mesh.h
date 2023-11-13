#pragma once

#include <span>
#include <array>

#include "glad/glad.h"

struct Vec3 {
    float x, y, z;
};

struct Texcoord2 {
    float u, v;
};

struct Colour {
    float r, g, b;
};

struct Vertex {
    Vec3 position;
    Texcoord2 texcoord;
    Colour colour;
};

struct VertexAttrib {
    GLint size;
    GLenum type;
    size_t offset;
};

struct Mesh {
    Mesh(std::span<const Vertex> vertices, std::span<const unsigned> indices);

    void bind();
    void draw();

private:
    size_t numVertices;
    size_t numIndices;
    unsigned vao;
    unsigned vbo;
    unsigned ebo;
};

Mesh loadObjMesh(const char* path);
