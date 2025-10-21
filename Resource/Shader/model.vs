#version 330 core

// --- 顶点属性输入 ---
// 从VAO中接收
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;
// location = 3, 4 (切线和副切线) 在这里暂时不用，但为以后保留

// --- Uniform 变量 ---
// 由C++代码在每帧或每次绘制时设置
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// 法线矩阵，用于在世界空间中正确变换法线
// 它是模型矩阵的左上3x3部分的逆转置矩阵
uniform mat3 u_NormalMatrix;

// --- 输出到片段着色器 ---
// 使用 'out' 关键字定义接口块
out VS_OUT {
    vec3 FragPos_World; // 片段在世界空间中的位置
    vec3 Normal_World;  // 片段在世界空间中的法线
    vec2 TexCoords;     // 纹理坐标
} vs_out;


void main()
{
    // 1. 将顶点位置变换到世界空间
    vs_out.FragPos_World = vec3(u_Model * vec4(a_Position, 1.0));
    
    // 2. 将法线变换到世界空间 (使用法线矩阵以处理非统一缩放)
    vs_out.Normal_World = normalize(u_NormalMatrix * a_Normal);

    // 3. 将纹理坐标直接传递出去
    vs_out.TexCoords = a_TexCoords;
    
    // 4. 计算最终的裁剪空间位置
    gl_Position = u_Projection * u_View * vec4(vs_out.FragPos_World, 1.0);
}