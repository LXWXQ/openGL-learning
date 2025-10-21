#version 330 core

// --- 输出 ---
out vec4 FragColor;

// --- 输入从顶点着色器 ---
in VS_OUT {
    vec3 FragPos_World;
    vec3 Normal_World;
    vec2 TexCoords;
} fs_in;

// --- Uniform 变量 ---

// 材质贴图 (扁平化命名，与模型加载器匹配)
uniform sampler2D texture_diffuse1;  // 漫反射贴图
uniform sampler2D texture_specular1; // 高光贴图
uniform sampler2D texture_normal1;   // 法线贴图 (暂时不用)
uniform float u_Shininess;           // 光泽度

// 光源属性
struct DirLight {
    vec3 direction;
    vec3 color;
};
uniform DirLight u_DirLight;

struct PointLight {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight u_PointLight;

// 相机位置
uniform vec3 u_ViewPos_World;

// --- 包含通用光照库 ---
#include "common/light.glsl"

void main()
{
    // --- 准备计算光照所需的数据 ---
    
    // 1. 材质颜色: 从漫反射贴图中采样
    vec3 albedo = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    
    // 2. 归一化法线
    vec3 N = normalize(fs_in.Normal_World);
    
    // 3. 计算观察向量
    vec3 V = normalize(u_ViewPos_World - fs_in.FragPos_World);

    // --- 计算光照 ---
    
    // 环境光
    vec3 ambient = 0.1 * albedo;
    
    // 分离漫反射和高光计算
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    // 方向光贡献
    vec3 L_dir = normalize(-u_DirLight.direction);
    totalDiffuse += lambert(N, L_dir) * u_DirLight.color * albedo;
    totalSpecular += blinnPhong(N, L_dir, V, u_Shininess) * u_DirLight.color;

    // 点光源贡献
    vec3 L_point = normalize(u_PointLight.position - fs_in.FragPos_World);
    float dist = length(u_PointLight.position - fs_in.FragPos_World);
    float attenuation = calcAttenuation(dist, u_PointLight.constant, 
                                       u_PointLight.linear, u_PointLight.quadratic);
    
    totalDiffuse += lambert(N, L_point) * u_PointLight.color * albedo * attenuation;
    totalSpecular += blinnPhong(N, L_point, V, u_Shininess) * u_PointLight.color * attenuation;

    // 从高光贴图获取强度
    float specularStrength = texture(texture_specular1, fs_in.TexCoords).r;
    
    // 最终颜色 = 环境光 + 漫反射 + (高光 * 高光强度)
    vec3 finalColor = ambient + totalDiffuse + totalSpecular * specularStrength;
    
    FragColor = vec4(finalColor, 1.0);
}