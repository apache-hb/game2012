#include "mesh.h"

#include "glad/glad.h"

Mesh::Mesh(std::span<const Vertex2> vertices, std::span<const unsigned> indices) {
    numIndices = indices.size();
    numVertices = vertices.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // bind and upload the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2), vertices.data(), GL_STATIC_DRAW);

    // our first 3 floats are the position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void*)offsetof(Vertex2, position));
    glEnableVertexAttribArray(0);

    // then colour
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void*)offsetof(Vertex2, colour));
    glEnableVertexAttribArray(1);

    // index buffer
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::bind() {
    glBindVertexArray(vao);
}

void Mesh::draw() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_POINTS, GLsizei(numIndices), GL_UNSIGNED_INT, 0);
}
