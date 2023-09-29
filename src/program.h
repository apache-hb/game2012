#pragma once

#include <string>

std::string loadFile(const char *path);

unsigned createShader(const char *vs, const char *fs);
