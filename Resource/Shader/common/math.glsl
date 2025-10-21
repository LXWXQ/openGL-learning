#ifndef MATH_GLSL
#define MATH_GLSL

const float PI = 3.14159265359;
const float TWO_PI = 6.28318530718;
const float HALF_PI = 1.57079632679;

// 饱和函数
float saturate(float x) 
{
    return clamp(x, 0.0, 1.0);
}

vec3 saturate(vec3 v)
 {
    return clamp(v, 0.0, 1.0);
}

// 平方
float pow2(float x) 
{
    return x * x;
}

// 五次方 (PBR 中常用)
float pow5(float x)
 {
    float x2 = x * x;
    return x2 * x2 * x;
}

// 线性到 sRGB
vec3 linearToSRGB(vec3 color)
 {
    return pow(color, vec3(1.0/2.2));
}

// sRGB 到线性
vec3 sRGBToLinear(vec3 color)
 {
    return pow(color, vec3(2.2));
}

#endif // MATH_GLSL