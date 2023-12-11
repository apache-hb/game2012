#version 330 core

out vec4 FragColor;

// vertex info
in vec2 texcoord;
in vec3 fragPos;
in vec3 normal;

// texture info
uniform sampler2D inBaseColour;
// green channel is roughness, blue channel is metallic, red channel is ambient occlusion
uniform sampler2D inMetallicRoughness;
uniform sampler2D inNormalMap;

// cant be bothered to do irradiance maps and stuff
// lets just pull from a skybox
uniform samplerCube inSkybox;

// ambient light
uniform float inAmbientLightStrength;
uniform vec3  inAmbientLightColour;

// point light
uniform vec3  inPointLightPosition;
uniform vec3  inPointLightColour;
uniform float inSpecularStrength;

// camera
uniform vec3  inCameraPosition;

#define PI 3.14159265359

vec3 get_normal() {
    vec3 tangent_normal = texture(inNormalMap, texcoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fragPos);
    vec3 Q2  = dFdy(fragPos);
    vec2 st1 = dFdx(texcoord);
    vec2 st2 = dFdy(texcoord);

    vec3 N   = normalize(normal);
    vec3 T   = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B   = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangent_normal);
}

float distribution_ggx(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float geometric_schlick_ggx(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometric_schlick_ggx(NdotV, roughness);
    float ggx1 = geometric_schlick_ggx(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnel_schlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnel_schlick_roughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main() {
    // material properties
    vec3 albedo = pow(texture(inBaseColour, texcoord).rgb, vec3(2.2));
    float metallic = texture(inMetallicRoughness, texcoord).b;
    float roughness = texture(inMetallicRoughness, texcoord).g;
    float ao = texture(inMetallicRoughness, texcoord).r;

    // get lighting information
    vec3 N = get_normal();
    vec3 V = normalize(inCameraPosition - fragPos);
    vec3 R = reflect(-V, N);

    // calculate reflectance
    vec3 F0 = mix(vec3(0.04), albedo.rgb, metallic);

    ///
    /// point light
    ///

    // only one light source for now
    vec3 L = normalize(inPointLightPosition - fragPos);
    vec3 H = normalize(V + L);
    float dist = length(inPointLightPosition - fragPos);
    float attenuation = 1.0 / (dist * dist);
    vec3 radiance = inPointLightColour * attenuation;

    // Cook-Torrance BRDF
    float NDF = distribution_ggx(N, H, roughness);
    float geom = geometry_smith(N, V, L, roughness);
    vec3 fresnel = fresnel_schlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * geom * fresnel;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = numerator / max(denominator, 0.001);

    vec3 ks = fresnel;
    vec3 kd = vec3(1.0) - ks;
    kd *= 1.0 - metallic;

    float ndotl = max(dot(N, L), 0.0);

    vec3 lo = (kd * albedo / PI + specular) * radiance * ndotl;

    vec3 f = fresnel_schlick_roughness(max(dot(N, V), 0.0), F0, roughness);

    ks = f;
    kd = 1.0 - ks;
    kd *= 1.0 - metallic;

    vec3 irradiance = texture(inSkybox, N).rgb;
    vec3 diffuse = irradiance * albedo;

    vec3 specular2 = specular * radiance * ndotl;

    vec3 ambient = (kd * diffuse + specular2) * ao;

    vec3 colour = ambient + lo;

    FragColor = vec4(colour, 1.0);
}
