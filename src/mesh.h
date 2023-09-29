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

struct Mesh {
    Mesh(std::span<const Vertex> vertices, std::span<const unsigned> indices);

    void bind();
    void draw();

private:
    size_t size;
    unsigned vao;
    unsigned vbo;
    unsigned ebo;
};
