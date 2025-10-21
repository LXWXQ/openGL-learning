#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

#include "Shader.h" 
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

// 顶点数据结构
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;      // 用于法线贴图
    glm::vec3 Bitangent;    // 用于法线贴图
};

// Mesh持有的纹理结构 (包含纹理指针和类型)
struct MeshTexture {
    TexturePtr texture;
    std::string type; // e.g., "texture_diffuse", "texture_specular"
};

class Mesh {
public:
    // Mesh 数据
    std::vector<Vertex>       vertices;
    std::vector<uint32_t>     indices;
    std::vector<MeshTexture>  textures;

    // 构造函数
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<MeshTexture> textures);

    // 绘制函数
    void draw(ShaderPtr shader);

private:
    // GPU 缓冲对象
    std::unique_ptr<VertexArray>  m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer>  m_EBO; // Element Buffer Object

    // 初始化GPU缓冲
    void setupMesh();
};

#endif // MESH_H