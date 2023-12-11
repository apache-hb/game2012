#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube inCubemap;

void main()
{
    FragColor = texture(inCubemap, TexCoords);
}
