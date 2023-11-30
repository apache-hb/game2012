#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexcoord0;

out vec2 texcoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4x4 inModel;
uniform mat4x4 inView;
uniform mat4x4 inProjection;

void main() {
    fragPos = vec3(inModel * vec4(inPosition, 1.0));
    normal = mat3(transpose(inverse(inModel))) * inNormal;
    texcoord = inTexcoord0;

    gl_Position = inProjection * inView * vec4(fragPos, 1.0);
}
