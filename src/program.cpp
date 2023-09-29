#include "program.h"

#include "glad/glad.h"

#include <iostream>
#include <fstream>

std::string loadFile(const char *path) {
    std::ifstream fd(path);
    if (!fd.is_open()) {
        std::cout << "Failed to open file: " << path << std::endl;
        std::abort();
    }

    std::string contents;
    fd.seekg(0, std::ios::end);
    contents.resize(fd.tellg());
    fd.seekg(0, std::ios::beg);
    fd.read(contents.data(), contents.size());
    fd.close();

    return contents;
}

namespace {
    template<typename FG, typename FL>
    void checkError(unsigned shader, const char *name, FG&& get, FL&& logs) {
        int ok = 0;
        get(shader, &ok);
        if (ok) { return; }

        char info[0x1000];
        logs(shader, 0x1000, info);
        std::cout << "Error in " << name << ": " << info << std::endl;
        std::abort();
    }

    void checkShader(unsigned shader, const char *name) {
        checkError(shader, name,
            [](auto shader, auto* ok) { return glGetShaderiv(shader, GL_COMPILE_STATUS, ok); },
            [](auto shader, auto len, auto* info) { return glGetShaderInfoLog(shader, len, nullptr, info); }
        );
    }
}

unsigned createShader(const char *vs, const char *fs) {
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vs, NULL);
    glCompileShader(vertexShader);

    checkShader(vertexShader, "vertex shader");

    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fs, NULL);
    glCompileShader(fragmentShader);

    checkShader(fragmentShader, "fragment shader");

    auto shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    checkError(shaderProgram, "shader program",
        [](auto shader, auto* ok) { return glGetProgramiv(shader, GL_LINK_STATUS, ok); },
        [](auto shader, auto len, auto* info) { return glGetProgramInfoLog(shader, len, nullptr, info); }
    );

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
