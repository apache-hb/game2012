#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <iostream>
#include <array>
#include <vector>

#include "program.h"
#include "mesh.h"

#include <cmath>

// square

constexpr auto kSquareIndicies = std::to_array<unsigned>({
    0, 1,
    1, 2,
    2, 3,
    3, 0
});

struct float2 {
    float x;
    float y;
};

// M_PI is annoying to get on windows
#define MY_PI 3.14159265358979323846f

float2 rotatePoint(float2 point, float2 origin, float angleInDeg, float scale) {
    float angleInRad = angleInDeg * (MY_PI / 180.f);

    float cosTheta = std::cos(angleInRad);
    float sinTheta = std::sin(angleInRad);

    float x = point.x - origin.x;
    float y = point.y - origin.y;

    float x1 = x * cosTheta - y * sinTheta;
    float y1 = x * sinTheta + y * cosTheta;

    return { (x1 + origin.x) * scale, (y1 + origin.y) * scale };
}

std::array<Vertex2, 4> makeSquare(float scale, float rotationInDeg) {
    float2 origin = { 0.0, 0.0 };

    float2 p1 = rotatePoint({ -1, -1 }, origin, rotationInDeg, scale);
    float2 p2 = rotatePoint({ 1, -1 }, origin, rotationInDeg, scale);
    float2 p3 = rotatePoint({ 1, 1 }, origin, rotationInDeg, scale);
    float2 p4 = rotatePoint({ -1, 1 }, origin, rotationInDeg, scale);

    return {
        Vertex2{ { p1.x, p1.y, 0.0f }, { 1.0f, 0.0f, 0.0f } },
        Vertex2{ { p2.x, p2.y, 0.0f }, { 0.0f, 1.0f, 0.0f } },
        Vertex2{ { p3.x, p3.y, 0.0f }, { 0.0f, 0.0f, 1.0f } },
        Vertex2{ { p4.x, p4.y, 0.0f }, { 1.0f, 1.0f, 1.0f } }
    };
}


void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void fbResize(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

std::vector<Mesh> genMeshes(float baseScale, float baseRotation, float scaleStep, float rotationStep, size_t steps) {
    std::vector<Mesh> squareMeshes = {};
    for (size_t i = 0; i < steps; i++) {
        auto squareVerts = makeSquare(baseScale, baseRotation);
        squareMeshes.push_back(Mesh(squareVerts, kSquareIndicies));

        baseRotation += rotationStep;
        baseScale /= scaleStep;
    }
    return squareMeshes;
}

// settings
constexpr unsigned int kWidth = 800;
constexpr unsigned int kHeight = 600;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(kWidth, kHeight, "hello", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, fbResize);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    float baseRotation = 0.f;
    float rotationStep = 45.f;

    float baseScale = 1.f;
    float scaleStep = 1.42f;
    int steps = 10;
    std::vector<Mesh> squareMeshes = genMeshes(baseScale, baseRotation, scaleStep, rotationStep, steps);

    auto vs = loadFile("data/square.vs.glsl");
    auto fs = loadFile("data/square.fs.glsl");

    unsigned shader = createShader(vs.c_str(), fs.c_str());

    // uncomment this call to draw in wireframe polygons.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ImVec4 clearColour = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Options");
            ImGui::ColorEdit3("Clear Colour", &clearColour.x);
            if (ImGui::SliderInt("Steps", &steps, 1, 100)) {
                squareMeshes = genMeshes(baseScale, baseRotation, scaleStep, rotationStep, steps);
            }

            if (ImGui::SliderFloat("Base scale", &baseScale, 0.1f, 1.f)) {
                squareMeshes = genMeshes(baseScale, baseRotation, scaleStep, rotationStep, steps);
            }

            if (ImGui::SliderFloat("Base rotation", &baseRotation, 0.f, 360.f)) {
                squareMeshes = genMeshes(baseScale, baseRotation, scaleStep, rotationStep, steps);
            }

            if (ImGui::SliderFloat("Scale step", &scaleStep, 1.1f, 2.f)) {
                squareMeshes = genMeshes(baseScale, baseRotation, scaleStep, rotationStep, steps);
            }

            if (ImGui::SliderFloat("Rotation step", &rotationStep, 0.f, 90.f)) {
                squareMeshes = genMeshes(baseScale, baseRotation, scaleStep, rotationStep, steps);
            }
        ImGui::End();

        glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        for (size_t i = 0; i < steps; i++) {
            squareMeshes[i].bind();
            squareMeshes[i].draw();
        }
        // draw via the index buffer
        //meshes[currentMesh].draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
