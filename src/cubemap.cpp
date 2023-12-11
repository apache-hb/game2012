#include "cubemap.h"

#include <stb/stb_image.h>

#include "glad/glad.h"

void copyTextureRegion(size_t x, size_t y, size_t width, size_t height, unsigned char *data, int channels, unsigned id) {
    unsigned char *tmp = new unsigned char[width * height * channels];

    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            size_t src = channels * (x + i + (y + j) * 2048);
            size_t dst = channels * (i + j * width);

            tmp[dst + 0] = data[src + 0];
            tmp[dst + 1] = data[src + 1];
            tmp[dst + 2] = data[src + 2];
            tmp[dst + 3] = data[src + 3];
        }
    }

    glTexImage2D(id, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp);
}

CubeMap::CubeMap(const char *path) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 4);

    // convert an equirectangular map into 6 faces of a cubemap
    // we assume each face is 512x512 and the image is 2048x1536

    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    // +x
    copyTextureRegion(1536, 512, 512, 512, data, 4, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    // -x
    copyTextureRegion(0, 512, 512, 512, data, 4, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    // +y
    copyTextureRegion(512, 0, 512, 512, data, 4, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    // -y
    copyTextureRegion(512, 1024, 512, 512, data, 4, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    // +z
    copyTextureRegion(512, 512, 512, 512, data, 4, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    // -z
    copyTextureRegion(1024, 512, 512, 512, data, 4, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // set the texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // clamp to edge to avoid seams
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear interpolation between mipmaps
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
}

void CubeMap::bind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void CubeMap::unbind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
