#version 330 core

out vec4 FragColor;

// vertex info
in vec2 texcoord;
in vec3 fragPos;
in vec3 normal;

// texture info
uniform float inRatio;
uniform sampler2D inTexture0;
uniform sampler2D inTexture1;

// ambient light
uniform float inAmbientLightStrength;
uniform vec3  inAmbientLightColour;

// point light
uniform vec3  inPointLightPosition;
uniform vec3  inPointLightColour;

uniform float inSpecularStrength;
uniform vec3  inCameraPosition;

void main() {
    // ambient light
    vec3 ambient = inAmbientLightStrength * inAmbientLightColour;

    // diffuse light

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(inPointLightPosition - fragPos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * inPointLightColour;

    // specular light
    vec3 view_dir = normalize(inCameraPosition - fragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = inSpecularStrength * spec * inPointLightColour;

    // texture
    vec4 tex = texture2D(inTexture0, texcoord) * inRatio
              + texture2D(inTexture1, texcoord) * (1.0 - inRatio);

    // final colour
    FragColor = vec4((ambient + diffuse + specular) * tex.rgb, tex.a);
}
