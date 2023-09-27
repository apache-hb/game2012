#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <iostream>
#include <array>
#include <vector>

#include "program.h"
#include "mesh.h"

// triangle

constexpr auto kTriangleVertexData = std::to_array<Vertex>({
    // positions      // colours
    { {-0.5f, -0.5f, 0.0f}, { 1.0f, 0.0f, 0.0 } },
    { {0.5f, -0.5f, 0.0f}, { 0.0f, 1.0f, 0.0 } },
    { {0.0f, 0.5f, 0.0f}, { 0.0f, 0.0f, 1.0 } }
});

constexpr auto kTriangleIndicies = std::to_array<unsigned>({
    0, 1, 2
});

// square

constexpr auto kSquareVertexData = std::to_array<Vertex>({
    // positions      // colours
    { {0.5f, 0.5f, 0.0f    }, { 1.0f, 0.0f, 0.0 } },
    { {0.5f, -0.5f, 0.0f,  }, { 0.0f, 1.0f, 0.0 } },
    { {-0.5f, -0.5f, 0.0f, }, { 0.0f, 0.0f, 1.0 } },
    { {-0.5f, 0.5f, 0.0f,  }, { 1.0f, 1.0f, 0.0 } }
});

constexpr auto kSquareIndicies = std::to_array<unsigned>({
    0, 1, 3,
    1, 2, 3
});

// circle

Mesh makeCircle(size_t points) {
    std::vector<Vertex> verts;
    std::vector<unsigned> indices;

    verts.reserve(points + 1);
    indices.reserve(points * 3);

    verts.push_back({ {0.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0 } });

    for (size_t i = 0; i < points; ++i) {
        float angle = 2.0f * 3.1415926f * float(i) / float(points);

        float x = std::cos(angle);
        float y = std::sin(angle);

        verts.push_back({ {x, y, 0.0f}, { 1.0f, 1.0f, 1.0 } });
    }

    for (unsigned i = 0; i < points; ++i) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }

    // stitch the last point to the first
    indices.push_back(0);
    indices.push_back(unsigned(points));
    indices.push_back(1);

    return Mesh(verts, indices);
}

// diamond

constexpr auto kDiamondVertexData = std::to_array<Vertex>({
    // positions      // colours
    { {0.5f, 0.0f, 0.0f    }, { 1.0f, 0.0f, 0.0 } },
    { {0.0f, 0.5f, 0.0f,  }, { 0.0f, 1.0f, 0.0 } },
    { {-0.5f, 0.0f, 0.0f, }, { 0.0f, 0.0f, 1.0 } },
    { {0.0f, -0.5f, 0.0f,  }, { 1.0f, 1.0f, 0.0 } }
});

constexpr auto kDiamondIndicies = std::to_array<unsigned>({
    0, 1, 2,
    0, 2, 3
});

constexpr auto kVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;

out vec3 ourColour;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColour = aColour;
}
)";

constexpr auto kFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec3 ourColour;

uniform int useVertexColour;
uniform vec3 colour;

void main() {
    if (useVertexColour != 0) {
        FragColor = vec4(ourColour, 1.0);
    } else {
        FragColor = vec4(colour, 1.0);
    }
}
)";

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void fbResize(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
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

    const char *meshNames[4] = { "Triangle", "Square", "Diamond", "Circle" };
    Mesh meshes[4] = {
        { kTriangleVertexData, kTriangleIndicies },
        { kSquareVertexData, kSquareIndicies },
        { kDiamondVertexData, kDiamondIndicies },
        makeCircle(32)
    };

    unsigned shader = createShader(kVertexShaderSource, kFragmentShaderSource);
    int toggleColourIndex = glGetUniformLocation(shader, "useVertexColour");
    int vertexColour = glGetUniformLocation(shader, "colour");

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ImVec4 clearColour = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);
    ImVec4 triangleColour = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
    int useVertexColour = false;
    int currentMesh = 0;
    bool animateColour = false;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Options");
            ImGui::ColorEdit3("Clear Colour", &clearColour.x);
            ImGui::ColorEdit3("Triangle Colour (Via UBO)", &triangleColour.x);

            bool it = useVertexColour;
            ImGui::Checkbox("Use Vertex Colour", &it);
            useVertexColour = it;

            ImGui::Checkbox("Animate Colour", &animateColour);

            ImGui::Combo("Mesh", &currentMesh, meshNames, IM_ARRAYSIZE(meshNames));

            ImGui::Text("1. is covered by the triangle");
            ImGui::Text("2. is covered using the `Use Vertex Colour` checkbox");
            ImGui::TextWrapped("3. is covered using the `Animate Colour` checkbox. You can also manually adjust the mesh and background colours using the colour pickers");
            ImGui::Text("consult the Mesh dropdown for 4 and 5");
        ImGui::End();

        glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        meshes[currentMesh].bind();

        if (animateColour) {
            float now = (float)glfwGetTime();
            glUniform3f(vertexColour,
                std::sin(now) * 0.5f + 0.5f,
                std::sin(now + 2.0f) * 0.5f + 0.5f,
                std::sin(now + 4.0f) * 0.5f + 0.5f
            );
            glUniform1i(toggleColourIndex, false);
        } else {
            glUniform3f(vertexColour, triangleColour.x, triangleColour.y, triangleColour.z);
            glUniform1i(toggleColourIndex, useVertexColour);
        }

        // draw via the index buffer
        meshes[currentMesh].draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
