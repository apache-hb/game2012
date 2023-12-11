#version 330

out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube inSkybox;

void main() {
    vec3 env = textureLod(inSkybox, WorldPos, 0.0).rgb;

    // gamma correction
    env = env / (env + vec3(1.0));
    env = pow(env, vec3(1.0 / 2.2));

    FragColor = vec4(env, 1.0);
}
