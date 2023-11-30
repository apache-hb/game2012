
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <iostream>
#include <array>
#include <vector>

#include "simcoe/core/panic.h"

#include "program.h"
#include "mesh.h"
#include "camera.h"
#include "gui.h"

Camera camera;

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    camera.processInput(window);
}

void processScroll(GLFWwindow*, double xoffset, double yoffset) {
    camera.processScroll(xoffset, yoffset);
}

void processMouse(GLFWwindow* w, double xpos, double ypos) {
    camera.processMouse(w, xpos, ypos);
}

void fbResize(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
    camera.screenWidth = float(width);
    camera.screenHeight = float(height);
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
    glfwSetCursorPosCallback(window, processMouse);
    glfwSetScrollCallback(window, processScroll);

    camera.screenWidth = kWidth;
    camera.screenHeight = kHeight;

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
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    auto vs = loadFile("data/model.vs.glsl");
    auto fs = loadFile("data/model.fs.glsl");

    unsigned shader = createShader(vs.c_str(), fs.c_str());

    // steal some models from my game for now
    Model model0 = {
        .mesh = loadObjMesh("data/monkey.model"),
        .tex0 = Texture("data/aaaa.png"),
        .tex1 = Texture("data/images/assets/player.png")
    };

    Model model1 = {
        .mesh = loadObjMesh("data/box.model"),
        .tex0 = Texture("data/BoomBoxWithAxes_baseColor.png"),
        .tex1 = Texture("data/images/store/logo44x44.png")
    };

    Model model2 = {
        .mesh = loadObjMesh("data/ring.model"),
        .tex0 = Texture("data/images/assets/cross.png"),
        .tex1 = Texture("data/images/store/logo44x44.png")
    };

    Model model3 = {
        .mesh = loadObjMesh("data/text.model"),
        .tex0 = Texture("data/images/assets/player.png"),
        .tex1 = Texture("data/images/assets/meme.png")
    };

    Model model4 = {
        .mesh = loadObjMesh("data/models/alien.model"),
        .tex0 = Texture("data/aaaa.png"),
        .tex1 = Texture("data/images/assets/player.png")
    };

    std::vector<Model> models = {
        model0, model1, model2, model3, model4
    };

    auto uModel = glGetUniformLocation(shader, "inModel");
    auto uView = glGetUniformLocation(shader, "inView");
    auto uProjection = glGetUniformLocation(shader, "inProjection");

    auto uTexture0 = glGetUniformLocation(shader, "inTexture0");
    auto uTexture1 = glGetUniformLocation(shader, "inTexture1");
    auto uRatio = glGetUniformLocation(shader, "inRatio");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUseProgram(shader);
    glEnable(GL_DEPTH_TEST);

    glUniform1i(uTexture0, 0);
    glUniform1i(uTexture1, 1);

    float ratio = 1.f;
    ImVec4 clearColour = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Options");
            ImGui::ColorEdit3("Clear Colour", &clearColour.x);

            ImGui::InputFloat3("position", &camera.cameraPos.x);
            ImGui::InputFloat3("direction", &camera.cameraFront.x);

            ImGui::SliderDegrees("fov", &camera.fov);
            ImGui::InputFloat("speed", &camera.speed);
            ImGui::InputFloat("sensitivity", &camera.sensitivity);

            ImGui::SliderFloat("ratio", &ratio, 0.0f, 1.f);

            for (size_t i = 0; i < models.size(); i++) {
                auto& model = models[i];
                ImGui::PushID(int(i));
                ImGui::Text("Model %zu", i);
                ImGui::SliderFloat3("position", &model.position.x, -10.f, 10.f);
                ImGui::SliderFloat3("rotation", &model.rotation.x, -180.f, 180.f);
                ImGui::SliderFloat3("scale", &model.scale.x, 0.1f, 10.f);
                ImGui::PopID();
            }
        ImGui::End();

        glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = camera.getView();
        auto projection = camera.getProjection();
        glUniformMatrix4fv(uView, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(uProjection, 1, GL_FALSE, &projection[0][0]);

        for (auto& m : models) {
            auto model = m.getModel();
            glUniformMatrix4fv(uModel, 1, GL_FALSE, &model[0][0]);

            glUniform1f(uRatio, ratio);

            m.tex0.bind(0);
            m.tex1.bind(1);

            // model0.mesh.bind();
            m.mesh.bind();
            m.mesh.draw();
        }
        // draw via the index buffer
        //meshes[currentMesh].draw();

        ImGui::EndFrame();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
