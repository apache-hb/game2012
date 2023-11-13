#pragma once

#include <span>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "glad/glad.h"

struct Vertex {
    glm::vec3 position;
    glm::vec2 texcoord;
};

template<>
struct std::hash<Vertex> {
    size_t operator()(const Vertex& vertex) const {
        return std::hash<glm::vec3>()(vertex.position) ^ std::hash<glm::vec2>()(vertex.texcoord);
    }
};

template<>
struct std::equal_to<Vertex> {
    bool operator()(const Vertex& lhs, const Vertex& rhs) const {
        return lhs.position == rhs.position && lhs.texcoord == rhs.texcoord;
    }
};

struct VertexAttrib {
    GLint size;
    GLenum type;
    size_t offset;
};

struct Mesh {
    Mesh(std::span<const Vertex> vertices, std::span<const uint16_t> indices);

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

struct Texture {
    Texture(const char* path);

    void bind(unsigned slot);

    unsigned id;
};

struct Model {
    Mesh mesh;
    Texture tex0;
    Texture tex1;
};
