#pragma once

#include <span>

struct Vec3 {
    float x, y, z;
};

struct Vec2 {
    float u, v;
};

struct Colour {
    float r, g, b;
};

struct Vertex {
    Vec3 position;
    Vec2 texcoord;
    Colour colour;
};

struct Vertex2 {
    Vec3 position;
    Colour colour;
};

struct Mesh {
    Mesh(std::span<const Vertex2> vertices, std::span<const unsigned> indices);

    void bind();
    void draw();

private:
    size_t numVertices;
    size_t numIndices;
    unsigned vao;
    unsigned vbo;
    unsigned ebo;
};
