#include "common.h"
#include "mesh.h"

#include "tinyobj/loader.h"

#include <filesystem>
#include <unordered_map>

Mesh loadObjMesh(const char* path) {
    auto cwd = std::filesystem::path(path).parent_path().string();

    // hastily copy pasted from my game engine and adapted to opengl
    std::vector<Vertex> vertexData;
    std::vector<uint16_t> indexData;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path, cwd.c_str());

    if (!warn.empty()) { info("tinyobj warn {}", warn); }
    if (!err.empty()) { error("tinyobj error {}", err); }

    if (!ok) {
        error("failed to load obj {}", path);
    }

    info("loaded obj {} (shapes={})", path, shapes.size());
    SM_ASSERTF(shapes.size() >= 1, "obj {} has no shapes", path);

    const auto& shape = shapes[0];
    std::unordered_map<Vertex, uint16_t> uniqueVertices;

    const auto& vertices = attrib.vertices;
    const auto& normals = attrib.normals;
    const auto& texcoords = attrib.texcoords;
    const auto& indices = shape.mesh.indices;
    SM_ASSERTF(vertices.size() % 3 == 0, "invalid vertex count {}", vertices.size());
    SM_ASSERTF(indices.size() % 3 == 0, "invalid index count {}", indices.size());

    info("(vertices={} normals={} uvs={} indices={})", vertices.size(), normals.size(), texcoords.size(), indices.size());

    auto getUvCoord = [&](int idx) {
        if (idx != -1) {
            auto c = float2{ texcoords[idx * 2 + 0], texcoords[idx * 2 + 1] };
            return c;
        } else {
            return float2{ 0.f, 0.f };
        }
    };

    auto getNormal = [&](int idx) {
        if (idx != -1) {
            auto c = float3{ normals[idx * 3 + 0], normals[idx * 3 + 1], normals[idx * 3 + 2] };
            return c;
        } else {
            return float3{ 0.f, 0.f, 0.f };
        }
    };

    auto getIndex = [&](tinyobj::index_t idx) {
        auto vtx = idx.vertex_index;
        auto uv = idx.texcoord_index;

        float2 uvCoord = getUvCoord(uv);
        float3 normal = getNormal(idx.normal_index);
        float3 position = { vertices[vtx * 3 + 0], vertices[vtx * 3 + 1], vertices[vtx * 3 + 2] };

        Vertex vertex = { position, normal, uvCoord };
        if (uniqueVertices.find(vertex) == uniqueVertices.end()) {
            uniqueVertices[vertex] = static_cast<uint16_t>(vertexData.size());
            vertexData.push_back(vertex);
        }

        return uniqueVertices[vertex];
    };

    size_t offset = 0;
    for (size_t i = 0; i < shape.mesh.num_face_vertices.size(); i++) {
        size_t verts = shape.mesh.num_face_vertices[i];
        if (verts == 3) {
            auto i0 = getIndex(indices[offset + 0]);
            auto i1 = getIndex(indices[offset + 1]);
            auto i2 = getIndex(indices[offset + 2]);

            indexData.push_back(i0);
            indexData.push_back(i1);
            indexData.push_back(i2);
        } else if (verts == 4) {
            auto i0 = getIndex(indices[offset + 0]);
            auto i1 = getIndex(indices[offset + 1]);
            auto i2 = getIndex(indices[offset + 2]);
            auto i3 = getIndex(indices[offset + 3]);

            indexData.push_back(i0);
            indexData.push_back(i1);
            indexData.push_back(i2);

            indexData.push_back(i0);
            indexData.push_back(i2);
            indexData.push_back(i3);
        } else {
            SM_ASSERTF(false, "unsupported face vertex count {}", verts);
        }

        offset += verts;
    }

    info("buffer sizes (vertices={} indices={})", vertexData.size(), indexData.size());

    return Mesh(vertexData, indexData);
}

struct ObjLoader {
    std::string cwd;

    // from tinyobj
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    // per shape
    std::vector<Vertex> vertexData;
    std::vector<uint16_t> indexData;

    Mesh loadMesh(size_t shape_idx) {
        const auto& shape = shapes[shape_idx];

        std::unordered_map<Vertex, uint16_t> uniqueVertices;

        const auto& vertices = attrib.vertices;
        const auto& normals = attrib.normals;
        const auto& texcoords = attrib.texcoords;
        const auto& indices = shape.mesh.indices;
        SM_ASSERTF(vertices.size() % 3 == 0, "invalid vertex count {}", vertices.size());
        SM_ASSERTF(indices.size() % 3 == 0, "invalid index count {}", indices.size());

        info("(vertices={} normals={} uvs={} indices={})", vertices.size(), normals.size(), texcoords.size(), indices.size());

        auto getUvCoord = [&](int idx) {
            if (idx != -1) {
                auto c = float2{ texcoords[idx * 2 + 0], texcoords[idx * 2 + 1] };
                return c;
            } else {
                return float2{ 0.f, 0.f };
            }
        };

        auto getNormal = [&](int idx) {
            if (idx != -1) {
                auto c = float3{ normals[idx * 3 + 0], normals[idx * 3 + 1], normals[idx * 3 + 2] };
                return c;
            } else {
                return float3{ 0.f, 0.f, 0.f };
            }
        };

        auto getIndex = [&](tinyobj::index_t idx) {
            auto vtx = idx.vertex_index;
            auto uv = idx.texcoord_index;

            float2 uvCoord = getUvCoord(uv);
            float3 normal = getNormal(idx.normal_index);
            float3 position = { vertices[vtx * 3 + 0], vertices[vtx * 3 + 1], vertices[vtx * 3 + 2] };

            Vertex vertex = { position, normal, uvCoord };
            if (uniqueVertices.find(vertex) == uniqueVertices.end()) {
                uniqueVertices[vertex] = static_cast<uint16_t>(vertexData.size());
                vertexData.push_back(vertex);
            }

            return uniqueVertices[vertex];
        };

        size_t offset = 0;
        for (size_t i = 0; i < shape.mesh.num_face_vertices.size(); i++) {
            size_t verts = shape.mesh.num_face_vertices[i];
            if (verts == 3) {
                auto i0 = getIndex(indices[offset + 0]);
                auto i1 = getIndex(indices[offset + 1]);
                auto i2 = getIndex(indices[offset + 2]);

                indexData.push_back(i0);
                indexData.push_back(i1);
                indexData.push_back(i2);
            } else if (verts == 4) {
                auto i0 = getIndex(indices[offset + 0]);
                auto i1 = getIndex(indices[offset + 1]);
                auto i2 = getIndex(indices[offset + 2]);
                auto i3 = getIndex(indices[offset + 3]);

                indexData.push_back(i0);
                indexData.push_back(i1);
                indexData.push_back(i2);

                indexData.push_back(i0);
                indexData.push_back(i2);
                indexData.push_back(i3);
            } else {
                SM_ASSERTF(false, "unsupported face vertex count {}", verts);
            }

            offset += verts;
        }

        info("buffer sizes (vertices={} indices={})", vertexData.size(), indexData.size());

        return Mesh(vertexData, indexData);
    }

    Material loadMaterial(size_t mat_idx) {
        const auto& material = materials[mat_idx];

        Material mat;

        if (!material.diffuse_texname.empty()) {
            std::string path = cwd + "/" + material.diffuse_texname;
            mat.base_colour = Texture(path.c_str());
        }

        if (!material.specular_texname.empty()) {
            std::string path = cwd + "/" + material.specular_texname;
            mat.metallic_roughness = Texture(path.c_str());
        }

        if (!material.bump_texname.empty()) {
            std::string path = cwd + "/" + material.bump_texname;
            mat.bump_map = Texture(path.c_str());
        }

        mat.emissive_colour = { material.emission[0], material.emission[1], material.emission[2] };
        mat.ambient_colour = { material.ambient[0], material.ambient[1], material.ambient[2] };

        mat.diffuse_colour = { material.diffuse[0], material.diffuse[1], material.diffuse[2] };

        mat.specular_colour = { material.specular[0], material.specular[1], material.specular[2] };
        mat.specular_exponent = material.shininess;

        mat.refraction_index = material.ior;
        mat.transparency = 1.f;

        return mat;

    }
};

ComplexModel loadObjModel(const char* path) {
    ObjLoader loader;
    loader.cwd = std::filesystem::path(path).parent_path().string();

    std::string warn;
    std::string err;

    bool ok = tinyobj::LoadObj(&loader.attrib, &loader.shapes, &loader.materials, &warn, &err, path, loader.cwd.c_str());

    if (!warn.empty()) { info("tinyobj warn {}", warn); }
    if (!err.empty()) { error("tinyobj error {}", err); }

    if (!ok) {
        error("failed to load obj {}", path);
    }

    size_t shape_count = loader.shapes.size();
    size_t material_count = loader.materials.size();

    info("loaded obj {} (shapes={} materials={})", path, shape_count, material_count);
    SM_ASSERTF(shape_count >= 1, "obj {} has no shapes", path);

    ComplexModel model;

    for (size_t i = 0; i < shape_count; i++) {
        const auto& shape = loader.shapes[i];

        Mesh mesh = loader.loadMesh(i);
        Material mat = loader.loadMaterial(shape.mesh.material_ids[0]);

        ComplexModel::Node node = {
            .mesh = mesh,
            .material = mat
        };

        model.nodes.push_back(node);
    }

    return model;
}
