#version 330 core

out vec4 FragColor;
in vec3 ourColour;
in vec2 uvCoords;

#define COLOUR_VERTEX 0
#define COLOUR_UNIFORM 1
#define COLOUR_PERLIN 2

uniform int colourMethod;
uniform vec3 colour;

float interpolate(float a0, float a1, float w) {
    return (a1 - a0) * w + a0;
}

vec2 randomGradient(int hash, float x, float y, float z) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    float rand = ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);

    return vec2(cos(rand), sin(rand));
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y) {
    // Get gradient from integer coordinates
    vec2 gradient = randomGradient(ix + iy, x, y, 0.0);

    // Compute the distance vector
    float dx = x - float(ix);
    float dy = y - float(iy);

    // Compute the dot-product
    return (dx*gradient.x + dy*gradient.y);
}

// Compute Perlin noise at coordinates x, y
float perlin(float x, float y) {
    // Determine grid cell coordinates
    int x0 = int(floor(x));
    int x1 = x0 + 1;
    int y0 = int(floor(y));
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - float(x0); // 0 <= sx <= 1
    float sy = y - float(y0); // 0 <= sy <= 1

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return value; // Will return in range -1 to 1. To make it in range 0 to 1, multiply by 0.5 and add 0.5
}

void main() {
    if (colourMethod == COLOUR_VERTEX) {
        FragColor = vec4(colour, 1.0);
    } else if (colourMethod == COLOUR_UNIFORM) {
        FragColor = vec4(ourColour, 1.0);
    } else if (colourMethod == COLOUR_PERLIN) {
        float noise = perlin(uvCoords.x * 10, uvCoords.y * 10);
        FragColor = vec4(noise, noise, noise, 1.0);
    } else {
        FragColor = vec4(uvCoords.x, uvCoords.y, 0.0, 1.0);
    }
}
