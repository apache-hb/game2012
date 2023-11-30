#include "mesh.h"

#include "glad/glad.h"

constexpr std::array<VertexAttrib, 3> vertexAttribs = {
    VertexAttrib{ 3, GL_FLOAT, offsetof(Vertex, position) },
    VertexAttrib{ 3, GL_FLOAT, offsetof(Vertex, normal) },
    VertexAttrib{ 2, GL_FLOAT, offsetof(Vertex, texcoord) }
};

Mesh::Mesh(std::span<const Vertex> vertices, std::span<const uint16_t> indices) {
    numIndices = indices.size();
    numVertices = vertices.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // bind and upload the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    for (GLuint i = 0; i < vertexAttribs.size(); i++) {
        const auto& attrib = vertexAttribs[i];
        glVertexAttribPointer(i, attrib.size, attrib.type, GL_FALSE, sizeof(Vertex), (void*)attrib.offset);
        glEnableVertexAttribArray(i);
    }

    // index buffer
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::bind() {
    glBindVertexArray(vao);
}

void Mesh::draw() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, GLsizei(numIndices), GL_UNSIGNED_SHORT, 0);
}
