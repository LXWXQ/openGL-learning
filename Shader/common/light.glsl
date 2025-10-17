#ifndef LIGHTING_GLSL
#define LIGHTING_GLSL

#include "math.glsl"

// Lambert 漫反射
float lambert(vec3 N, vec3 L) {
    return max(dot(N, L), 0.0);
}

// Blinn-Phong 高光
float blinnPhong(vec3 N, vec3 L, vec3 V, float shininess) {
    vec3 H = normalize(L + V);
    float NdotH = max(dot(N, H), 0.0);
    return pow(NdotH, shininess);
}

// 衰减计算
float calcAttenuation(float distance, float constant, float linear, float quadratic) {
    return 1.0 / (constant + linear * distance + quadratic * distance * distance);
}

// 方向光
vec3 calcDirectionalLight(vec3 direction, vec3 color, vec3 N, vec3 V, 
                          vec3 albedo, float shininess) {
    vec3 L = normalize(-direction);
    
    // 漫反射
    float diff = lambert(N, L);
    vec3 diffuse = diff * color * albedo;
    
    // 高光
    float spec = blinnPhong(N, L, V, shininess);
    vec3 specular = spec * color;
    
    return diffuse + specular;
}

// 点光源
vec3 calcPointLight(vec3 position, vec3 color, float constant, float linear, float quadratic,
                    vec3 fragPos, vec3 N, vec3 V, vec3 albedo, float shininess) {
    vec3 L = normalize(position - fragPos);
    float distance = length(position - fragPos);
    
    // 衰减
    float attenuation = calcAttenuation(distance, constant, linear, quadratic);
    
    // 漫反射
    float diff = lambert(N, L);
    vec3 diffuse = diff * color * albedo;
    
    // 高光
    float spec = blinnPhong(N, L, V, shininess);
    vec3 specular = spec * color;
    
    return (diffuse + specular) * attenuation;
}

#endif // LIGHTING_GLSL
