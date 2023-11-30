#pragma once

#include <span>
#include <array>

#include "simcoe/math/math.h"
#include "simcoe/math/hash.h"

#include "glad/glad.h"

using namespace math;

struct Vertex {
    float3 position;
    float2 texcoord;
};

template<>
struct std::hash<Vertex> {
    size_t operator()(const Vertex& vertex) const {
        return std::hash<float3>()(vertex.position)
             ^ std::hash<float2>()(vertex.texcoord);
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
    float3 position = { 0.f, 0.f, 0.f };

    /// stored in Degrees, couldnt get the template to work properly so trust me on this one
    float3 rotation = { 0.f, 0.f, 0.f };
    float3 scale = { 1.f, 1.f, 1.f };

    float4x4 getModel() const {
        return float4x4::transform(position, rotation * kDegToRad<float>, scale);
    }

    Mesh mesh;
    Texture tex0;
    Texture tex1;
};
