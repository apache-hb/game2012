#version 330 core

out vec4 FragColor;

// vertex info
in vec2 texcoord;
in vec3 fragPos;
in vec3 normal;

// texture info
uniform sampler2D inBaseColour;
uniform sampler2D inMetallicRoughness; // green channel is roughness, blue channel is metallic
uniform sampler2D inNormalMap;

// ambient light
uniform float inAmbientLightStrength;
uniform vec3  inAmbientLightColour;

// point light
uniform vec3  inPointLightPosition;
uniform vec3  inPointLightColour;
uniform float inSpecularStrength;

// camera
uniform vec3  inCameraPosition;

void main() {
    // base colour
    vec4 baseColour = texture2D(inBaseColour, texcoord);

    // metallic roughness
    vec4 metallicRoughness = texture2D(inMetallicRoughness, texcoord);

    // grab the roughness and metallic values from the texture
    // to act as our specular map

    // roughness is basically the inverse of the specular power
    float roughness = metallicRoughness.g;

    // metallic is the amount of specular reflection
    float metallic = metallicRoughness.b;

    // ambient light
    vec3 ambient = inAmbientLightStrength * inAmbientLightColour * baseColour.rgb;

    // diffuse light
    vec3 light_dir = normalize(inPointLightPosition - fragPos);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * inPointLightColour;

    // specular light
    vec3 view_dir = normalize(inCameraPosition - fragPos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), roughness);
    vec3 specular = inSpecularStrength * spec * inPointLightColour;

    // final colour
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
