#version 330 core

out vec4 FragColor;
in vec3 WorldPos;

uniform sampler2D cubemap;

const vec2 invatan = vec2(0.1591, 0.3183);
vec2 sampleSpherical(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invatan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec2 uv = sampleSpherical(normalize(WorldPos));
    vec3 colour = texture(cubemap, uv).rgb;
    FragColor = vec4(colour, 1.0);
}
