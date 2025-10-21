#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<MeshTexture> textures)
    : vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures)) {
    // 构造函数接收数据后，立即设置GPU缓冲
    setupMesh();
}

void Mesh::setupMesh() {
    // 1. 创建缓冲对象
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex));
    m_EBO = std::make_unique<IndexBuffer>(indices.data(), indices.size());

    // 2. 设置顶点布局
    VertexBufferLayout layout;
    // a_Position
    layout.push<float>(3);
    // a_Normal
    layout.push<float>(3);
    // a_TexCoords
    layout.push<float>(2);
    // a_Tangent
    layout.push<float>(3);
    // a_Bitangent
    layout.push<float>(3);

    // 3. 将 VBO 和布局添加到 VAO
    m_VAO->addBuffer(*m_VBO, layout);
}

void Mesh::draw(ShaderPtr shader) {
    // 在绘制前绑定此Mesh需要的所有纹理
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        std::string number;
        std::string name = textures[i].type; // "texture_diffuse" or "texture_specular"

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);

        // 设置 uniform (e.g., "texture_diffuse1", "texture_specular2")
        shader->setInt((name + number).c_str(), i);
        // 绑定纹理到对应的纹理单元
        textures[i].texture->bind(i);
    }

    // 绘制 Mesh
    m_VAO->bind();
    m_EBO->bind();
    glDrawElements(GL_TRIANGLES, m_EBO->getCount(), GL_UNSIGNED_INT, 0);
    m_EBO->unbind();
    m_VAO->unbind();

    // 绘制后最好将纹理单元恢复默认
    glActiveTexture(GL_TEXTURE0);
}