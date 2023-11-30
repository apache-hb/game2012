#pragma once

#include <span>
#include <array>
#include <vector>

#include "simcoe/math/math.h"
#include "simcoe/math/hash.h"

#include "glad/glad.h"

using namespace math;

struct Vertex {
    float3 position;
    float3 normal;
    float2 texcoord;
};

template<>
struct std::hash<Vertex> {
    size_t operator()(const Vertex& vertex) const {
        return std::hash<float3>()(vertex.position)
             ^ std::hash<float3>()(vertex.normal)
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
    Texture();
    Texture(const char* path);

    void generate();

    void bind(unsigned slot);

    unsigned id;
};

struct Material {
    Texture base_colour; // map_Kd

    /// map_Ks, green channel is roughness, blue channel is metallic
    Texture metallic_roughness;

    Texture bump_map; // map_Bump

    float3 emissive_colour = { 0.f, 0.f, 0.f }; /// Ke

    float3 ambient_colour = { 1.f, 1.f, 1.f }; /// Ka

    float3 diffuse_colour = { 1.f, 1.f, 1.f }; /// Kd

    float3 specular_colour = { 1.f, 1.f, 1.f }; /// Ks
    float specular_exponent = 1.f; /// Ns

    float refraction_index = 1.f; /// Ni

    float transparency = 1.f; /// d

    // assume illum 2
};

struct ComplexModel {
    float3 position = { 0.f, 0.f, 0.f };
    float3 rotation = { 0.f, 0.f, 0.f };
    float3 scale = { 1.f, 1.f, 1.f };

    float4x4 getModel() const {
        return float4x4::transform(position, rotation * kDegToRad<float>, scale).transpose();
    }

    struct Node {
        Mesh mesh;
        Material material;
    };

    std::vector<Node> nodes;
};

ComplexModel loadObjModel(const char* path);

struct Model {
    float3 position = { 0.f, 0.f, 0.f };

    /// stored in Degrees, couldnt get the template to work properly so trust me on this one
    float3 rotation = { 0.f, 0.f, 0.f };
    float3 scale = { 1.f, 1.f, 1.f };

    float4x4 getModel() const {
        // we operate on row major matrices, opengl wants column major
        // transpose the matrix before returning it
        return float4x4::transform(position, rotation * kDegToRad<float>, scale).transpose();
    }

    Mesh mesh;
    Texture tex0;
    Texture tex1;
};

struct PointLight {
    /// position of the light
    float3 position = { 0.f, 0.f, 0.f };
    float3 colour = { 1.f, 1.f, 1.f };

    float strength = 1.f;
};

struct AmbientLight {
    float strength = 1.f;
    float3 colour = { 1.f, 1.f, 1.f };
};
