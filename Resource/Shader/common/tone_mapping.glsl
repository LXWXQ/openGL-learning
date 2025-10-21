#ifndef TONE_MAPPING_GLSL
#define TONE_MAPPING_GLSL

// Reinhard tone mapping
vec3 reinhardToneMapping(vec3 color) {
    return color / (color + vec3(1.0));
}

// Exposure tone mapping
vec3 exposureToneMapping(vec3 color, float exposure) {
    return vec3(1.0) - exp(-color * exposure);
}

// ACES Filmic tone mapping
vec3 ACESFilm(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}

// Uncharted 2 tone mapping
vec3 uncharted2ToneMapping(vec3 color) {
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;
    float W = 11.2;
    
    color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
    float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
    color /= white;
    
    return color;
}

#endif // TONE_MAPPING_GLSL