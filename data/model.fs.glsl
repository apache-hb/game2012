#version 330 core

out vec4 FragColor;

in vec2 texcoord;

uniform float inRatio;

uniform sampler2D inTexture0;
uniform sampler2D inTexture1;

void main() {
    FragColor = texture2D(inTexture0, texcoord) * inRatio 
              + texture2D(inTexture1, texcoord) * (1.0 - inRatio);
}
