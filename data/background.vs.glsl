#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 inProjection;
uniform mat4 inView;

out vec3 WorldPos;

void main() {
    WorldPos = pos;

    mat4 view = mat4(mat3(inView));
    vec3 clip = projection * view * vec4(pos, 1.0);

    gl_Position = vec4(clip.xy, 0.0, 1.0);
}
