#pragma once

#include <glm/glm.hpp>

// use the same coordinate system as learnopengl
// i would've preferred to use the same as my engine but its all d3d12
// and while im confident i could make it work with opengl for this project
// i dont have the time to do it right now.
constexpr glm::vec3 kUp = glm::vec3(0.0f, 1.0f, 0.0f);
constexpr glm::vec3 kRight = glm::vec3(1.0f, 0.0f, 0.0f);

struct Camera {
    // perspective projection only

    float fov = 90.f;
    float near = 0.1f;
    float far = 2000.f;

    glm::vec3 position = { 0.f, 0.f, 0.f };
    glm::vec3 direction = { 0.f, 0.f, -1.f };
};
