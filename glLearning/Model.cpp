#include "Model.h"
#include "Texture.h" // 确保Texture类实现中包含了stb_image
#include <iostream>

Model::Model(const std::string& path) {
	loadModel(path);
}

void Model::draw(ShaderPtr shader) {
	for (unsigned int i = 0; i < m_meshes.size(); i++) {
		m_meshes[i].draw(shader);
	}
}

void Model::loadModel(const std::string& path) {
	Assimp::Importer importer;
	// aiProcess_Triangulate: 将所有图元转为三角形
	// aiProcess_FlipUVs: 翻转Y轴的纹理坐标 (OpenGL需要)
	// aiProcess_CalcTangentSpace: 为法线贴图计算切线和副切线
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	m_directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// 处理节点自身的所有网格
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}
	// 递归处理所有子节点
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<MeshTexture> textures;
    // 1. 处理顶点
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // 位置
        vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        // 法线
        if (mesh->HasNormals()) {
            vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        }
        // 纹理坐标 (只取第一个通道)
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
            // 切线和副切线
            vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
            vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    // 2. 处理索引
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // 3. 处理材质和纹理
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // 漫反射贴图
        std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // 镜面光/高光贴图
        std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        // 法线贴图 (在Assimp中常被归为高度图)
        std::vector<MeshTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
    std::vector<MeshTexture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        // --- 核心修改在这里 ---
        // 1. 在检查缓存之前，先构建出完整的、唯一的纹理路径
        std::string fullPath = m_directory + '/' + std::string(str.C_Str());

        // 2. 检查这个完整路径是否已经在我们的缓存中了
        bool skip = false;
        for (unsigned int j = 0; j < m_texturesLoaded.size(); j++)
        {
            // 使用 fullPath 和缓存中的完整路径进行比较
            if (m_texturesLoaded[j].texture->getPath() == fullPath)
            {
                textures.push_back(m_texturesLoaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            // 如果纹理没被加载过，则加载它
            // 注意：我们现在使用 fullPath 来加载，而不是之前的 texturePath

            Texture::Parameters params;
            params.generateMipmaps = true;
            if (typeName == "texture_diffuse")
            {
                params.sRGB = true;
            }

            // 使用 fullPath 创建 Texture 对象
            auto texture = std::make_shared<Texture>(fullPath, params);

            MeshTexture meshTex;
            meshTex.texture = texture;
            meshTex.type = typeName;

            textures.push_back(meshTex);
            m_texturesLoaded.push_back(meshTex); // 添加到已加载列表
        }
    }
    return textures;
}

// 在Model.h中为Texture类添加一个友元声明或公开m_path，以便在上面代码中访问
// 例如在Texture.h的Texture类中添加:
// public: std::string m_path; (简单起见)
// 或者提供一个 getPath() const 方法  
// **注意:** `loadMaterialTextures` 函数中需要访问 `Texture` 对象的路径来判断是否重复。你需要在你的 `Texture` 类中提供一个公共的 `getPath()` 方法或者将 `m_path` 设为 public。
