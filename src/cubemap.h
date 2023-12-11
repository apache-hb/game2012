#pragma once

struct CubeMap {
    CubeMap(const char* path);
    void bind();
    void unbind();

    unsigned id;
};
