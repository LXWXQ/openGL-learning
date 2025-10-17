#ifndef TRANSFORMS_GLSL
#define TRANSFORMS_GLSL

// TBN 矩阵构建
mat3 computeTBN(vec3 N, vec3 tangent, vec3 bitangent) {
    return mat3(tangent, bitangent, N);
}

// 法线贴图采样
vec3 sampleNormalMap(sampler2D normalMap, vec2 texCoords, mat3 TBN) {
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normal * 2.0 - 1.0; // [0,1] -> [-1,1]
    return normalize(TBN * normal);
}

// 视差贴图 (Parallax Mapping)
vec2 parallaxMapping(sampler2D depthMap, vec2 texCoords, vec3 viewDir, float heightScale) {
    float height = texture(depthMap, texCoords).r;
    vec2 p = viewDir.xy / viewDir.z * (height * heightScale);
    return texCoords - p;
}

// 陡峭视差贴图 (Steep Parallax Mapping)
vec2 steepParallaxMapping(sampler2D depthMap, vec2 texCoords, vec3 viewDir, float heightScale) {
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;
    
    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
    
    while(currentLayerDepth < currentDepthMapValue) {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }
    
    return currentTexCoords;
}

#endif // TRANSFORMS_GLSL
