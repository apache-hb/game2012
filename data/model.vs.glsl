#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexcoord0;

out vec2 texcoord;

uniform mat4x4 inModel;
uniform mat4x4 inView;
uniform mat4x4 inProjection;

void main() {
    gl_Position = inProjection * inView * inModel * vec4(inPosition, 1.0);
    texcoord = inTexcoord0;
}
