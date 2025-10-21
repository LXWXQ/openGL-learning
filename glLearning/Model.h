#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

class Model {
public:
    // 构造函数，接收文件路径
    Model(const std::string& path);

    // 绘制模型（会绘制其所有的Mesh）
    void draw(ShaderPtr shader);

private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    // 用于缓存已加载的纹理，避免重复加载
    std::vector<MeshTexture> m_texturesLoaded;

    // --- Assimp 加载函数 ---
    void loadModel(const std::string& path);
    // 递归处理节点
    void processNode(aiNode* node, const aiScene* scene);
    // 处理网格数据
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    // 加载材质中的纹理
    std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};

#endif // MODEL_H