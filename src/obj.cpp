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
    const auto& texcoords = attrib.texcoords;
    const auto& indices = shape.mesh.indices;
    SM_ASSERTF(vertices.size() % 3 == 0, "invalid vertex count {}", vertices.size());
    SM_ASSERTF(indices.size() % 3 == 0, "invalid index count {}", indices.size());

    info("(vertices={} uvs={} indices={})", vertices.size(), texcoords.size(), indices.size());

    auto getUvCoord = [&](int idx) {
        if (idx != -1) {
            auto c = glm::vec2{ texcoords[idx * 2 + 0], texcoords[idx * 2 + 1] };
            return c;
        } else {
            return glm::vec2{ 0.f, 0.f };
        }
    };

    auto getIndex = [&](tinyobj::index_t idx) {
        auto vtx = idx.vertex_index;
        auto uv = idx.texcoord_index;

        glm::vec2 uvCoord = getUvCoord(uv);
        glm::vec3 position = { vertices[vtx * 3 + 0], vertices[vtx * 3 + 1], vertices[vtx * 3 + 2] };

        Vertex vertex = { position, uvCoord };
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
