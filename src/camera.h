#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "simcoe/core/panic.h"
#include "simcoe/math/math.h"

using namespace math;

struct Camera {
    // camera
    float3 cameraPos   = float3(0.0f, 0.0f, 3.0f);
    float3 cameraFront = float3(0.0f, 0.0f, -1.0f);
    float3 cameraUp    = float3(0.0f, 1.0f, 0.0f);

    bool firstMouse = true;
    Degrees<float> yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    Degrees<float> pitch =  0.0f;
    float lastX =  800.0f / 2.0;
    float lastY =  600.0 / 2.0;
    Degrees<float> fov   =  45.0f;

    float screenWidth = 800.0f;
    float screenHeight = 600.0f;

    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

    // speed
    float speed = 2.5f;
    float sensitivity = 0.1f;

    float4x4 getModel() {
        return float4x4::identity();
    }

    float4x4 getView() {
        return float4x4::lookAtRH(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    float4x4 getCubemapView() {
        return float4x4(float3x3(getView()));
    }

    float4x4 getProjection() {
        return float4x4::perspectiveRH(math::radians(fov), screenWidth / screenHeight, 0.1f, 100.0f);
    }

    void processInput(GLFWwindow *window) {
        float now = (float)glfwGetTime();
        deltaTime = now - lastFrame;
        lastFrame = now;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        float cameraSpeed = static_cast<float>(speed * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraFront * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraFront * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= float3::cross(cameraFront, cameraUp).normal() * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += float3::cross(cameraFront, cameraUp).normal() * cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            cameraPos.y += cameraSpeed;
        else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cameraPos.y -= cameraSpeed;
    }

    void processMouse(GLFWwindow *window, double xposIn, double yposIn) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {
            return;
        }

        yaw += math::degrees(xoffset);
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        float3 front;
        front.x = cosf(math::radians(yaw)) * cosf(math::radians(pitch));
        front.y = sinf(math::radians(pitch));
        front.z = sinf(math::radians(yaw)) * cosf(math::radians(pitch));
        cameraFront = front.normal();
    }

    void processScroll(double, double yoffset) {
        fov -= math::degrees(float(yoffset));
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }
};
