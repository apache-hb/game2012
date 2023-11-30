
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

    ///
    /// complex setup
    ///

    auto c_vs = loadFile("data/helmet.vs.glsl");
    auto c_fs = loadFile("data/helmet.fs.glsl");

    unsigned c_shader = createShader(c_vs.c_str(), c_fs.c_str());

    ComplexModel c_helmet = loadObjModel("data/flight_helmet/helmet.model");

    auto c_uModel = glGetUniformLocation(c_shader, "inModel");
    auto c_uView = glGetUniformLocation(c_shader, "inView");
    auto c_uProjection = glGetUniformLocation(c_shader, "inProjection");

    auto c_uBaseColour = glGetUniformLocation(c_shader, "inBaseColour");
    auto c_uMetallicRoughness = glGetUniformLocation(c_shader, "inMetallicRoughness");
    auto c_uNormalMap = glGetUniformLocation(c_shader, "inNormalMap");

    auto c_uAmbientLightStrength = glGetUniformLocation(c_shader, "inAmbientLightStrength");
    auto c_uAmbientLightColour = glGetUniformLocation(c_shader, "inAmbientLightColour");

    auto c_uPointLightPosition = glGetUniformLocation(c_shader, "inPointLightPosition");
    auto c_uPointLightColour = glGetUniformLocation(c_shader, "inPointLightColour");

    auto c_uSpecularStrength = glGetUniformLocation(c_shader, "inSpecularStrength");
    auto c_uCameraPos = glGetUniformLocation(c_shader, "inCameraPosition");

    // lights

    AmbientLight ambientLight = {
        .strength = 0.1f,
        .colour = { 1.f, 1.f, 1.f }
    };

    PointLight pointLight = {
        .position = { 0.f, 0.f, 0.f },
        .colour = { 1.f, 1.f, 1.f }
    };

    ///
    /// simple setup
    ///

    Model model0 = {
        .mesh = loadObjMesh("data/monkey.model"),
        .tex0 = Texture("data/aaaa.png"),
        .tex1 = Texture("data/images/assets/player.png")
    };

    std::vector<Model> models = {
        model0
    };

    Model the_blue_monkey = model0;
    the_blue_monkey.tex0 = Texture("data/images/assets/alien.png");

    auto s_vs = loadFile("data/model.vs.glsl");
    auto s_fs = loadFile("data/model.fs.glsl");

    unsigned s_shader = createShader(s_vs.c_str(), s_fs.c_str());

    auto s_uModel = glGetUniformLocation(s_shader, "inModel");
    auto s_uView = glGetUniformLocation(s_shader, "inView");
    auto s_uProjection = glGetUniformLocation(s_shader, "inProjection");

    auto s_uTexture0 = glGetUniformLocation(s_shader, "inTexture0");
    auto s_uTexture1 = glGetUniformLocation(s_shader, "inTexture1");
    auto s_uRatio = glGetUniformLocation(s_shader, "inRatio");

    auto s_uAmbientLightStrength = glGetUniformLocation(s_shader, "inAmbientLightStrength");
    auto s_uAmbientLightColour = glGetUniformLocation(s_shader, "inAmbientLightColour");

    auto s_uPointLightPosition = glGetUniformLocation(s_shader, "inPointLightPosition");
    auto s_uPointLightColour = glGetUniformLocation(s_shader, "inPointLightColour");

    auto s_uSpecularStrength = glGetUniformLocation(s_shader, "inSpecularStrength");
    auto s_uCameraPos = glGetUniformLocation(s_shader, "inCameraPosition");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);


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

            // do the ##stuff because of imguis id stack

            ImGui::SeparatorText("Ambient Light");
            ImGui::SliderFloat("strength##ambient", &ambientLight.strength, 0.f, 1.f);
            ImGui::ColorEdit3("colour##ambient", &ambientLight.colour.x);

            ImGui::SeparatorText("Point Light");
            ImGui::SliderFloat3("position##point", &pointLight.position.x, -10.f, 10.f);
            ImGui::ColorEdit3("colour##point", &pointLight.colour.x);
            ImGui::SliderFloat("strength##point", &pointLight.strength, 0.f, 1.f);

            for (size_t i = 0; i < models.size(); i++) {
                auto& model = models[i];
                ImGui::PushID(int(i));

                char label[32];
                sprintf(label, "Model %zu", i);
                ImGui::SeparatorText(label);

                ImGui::SliderFloat3("position", &model.position.x, -10.f, 10.f);
                ImGui::SliderFloat3("rotation", &model.rotation.x, -180.f, 180.f);
                ImGui::SliderFloat3("scale", &model.scale.x, 0.1f, 10.f);
                ImGui::PopID();
            }
        ImGui::End();

        glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ///
        /// draw simple
        ///
        glUseProgram(s_shader);

        glUniform1i(s_uTexture0, 0);
        glUniform1i(s_uTexture1, 1);

        auto view = camera.getView();
        auto projection = camera.getProjection();
        glUniformMatrix4fv(s_uView, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(s_uProjection, 1, GL_FALSE, &projection[0][0]);

        glUniform3fv(s_uAmbientLightColour, 1, ambientLight.colour.data());
        glUniform3fv(s_uCameraPos, 1, camera.cameraPos.data());
        glUniform1f(s_uSpecularStrength, pointLight.strength);

        glUniform3fv(s_uPointLightPosition, 1, pointLight.position.data());
        glUniform3fv(s_uPointLightColour, 1, pointLight.colour.data());

        // draw the blue monkey
        the_blue_monkey.position = pointLight.position;
        the_blue_monkey.scale = { 0.1f, 0.1f, 0.1f };

        the_blue_monkey.tex0.bind(0);
        the_blue_monkey.tex0.bind(1);

        glUniform1f(s_uAmbientLightStrength, 1.f);

        glUniform1f(s_uRatio, 1.f);
        glUniformMatrix4fv(s_uModel, 1, GL_FALSE, &the_blue_monkey.getModel()[0][0]);
        the_blue_monkey.mesh.bind();
        the_blue_monkey.mesh.draw();

        glUniform1f(s_uAmbientLightStrength, ambientLight.strength);

        // draw the models
        for (auto& m : models) {
            auto model = m.getModel();
            glUniformMatrix4fv(s_uModel, 1, GL_FALSE, &model[0][0]);

            glUniform1f(s_uRatio, ratio);

            m.tex0.bind(0);
            m.tex1.bind(1);

            // model0.mesh.bind();
            m.mesh.bind();
            m.mesh.draw();
        }
        // draw via the index buffer
        //meshes[currentMesh].draw();

        ///
        /// draw complex here
        ///
        glUseProgram(c_shader);

        // textures
        glUniform1i(c_uBaseColour, 0);
        glUniform1i(c_uMetallicRoughness, 1);
        glUniform1i(c_uNormalMap, 2);

        view = camera.getView();
        projection = camera.getProjection();
        auto model = c_helmet.getModel();

        // camera view and projection
        glUniformMatrix4fv(c_uView, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(c_uProjection, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(c_uModel, 1, GL_FALSE, &model[0][0]);

        // ambient
        glUniform1f(c_uAmbientLightStrength, ambientLight.strength);
        glUniform3fv(c_uAmbientLightColour, 1, ambientLight.colour.data());

        // point light
        glUniform3fv(c_uPointLightPosition, 1, pointLight.position.data());
        glUniform3fv(c_uPointLightColour, 1, pointLight.colour.data());
        glUniform1f(c_uSpecularStrength, pointLight.strength);

        // camera
        glUniform3fv(c_uCameraPos, 1, camera.cameraPos.data());

        for (auto& node : c_helmet.nodes) {
            node.material.base_colour.bind(0);
            node.material.metallic_roughness.bind(1);
            node.material.bump_map.bind(2);

            node.mesh.bind();
            node.mesh.draw();
        }

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
