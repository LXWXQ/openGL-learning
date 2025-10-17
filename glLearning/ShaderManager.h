#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "Shader.h" // 包含你写的Shader头文件
#include <string>
#include <unordered_map>
#include <memory>

// ShaderManager是一个单例类，负责加载、存储和提供对Shader对象的访问
class ShaderManager {
public:
    // 获取单例实例的静态方法
    static ShaderManager& getInstance();

    // 禁止拷贝和赋值，以维护单例的唯一性
    ShaderManager(const ShaderManager&) = delete;
    void operator=(const ShaderManager&) = delete;

    /**
     * @brief 加载(或获取已加载的)一个着色器程序.
     * @param name 用于在管理器中唯一标识此着色器的别名.
     * @param vertexPath 顶点着色器文件路径.
     * @param fragmentPath 片段着色器文件路径.
     * @return 返回一个指向Shader的共享指针. 如果加载失败则返回nullptr.
     */
    ShaderPtr load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

    /**
     * @brief 加载(或获取已加载的)一个带几何着色器的着色器程序.
     * @param name 用于在管理器中唯一标识此着色器的别名.
     * @param vertexPath 顶点着色器文件路径.
     * @param fragmentPath 片段着色器文件路径.
     * @param geometryPath 几何着色器文件路径.
     * @return 返回一个指向Shader的共享指针. 如果加载失败则返回nullptr.
     */
    ShaderPtr load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);

    /**
     * @brief 通过别名获取一个已经加载的着色器.
     * @param name 着色器的别名.
     * @return 返回一个指向Shader的共享指针. 如果未找到则返回nullptr.
     */
    ShaderPtr get(const std::string& name) const;

    /**
     * @brief 重新加载所有已注册的着色器. 用于实时调试.
     */
    void reloadAll();

    /**
     * @brief 清理所有已加载的着色器资源.
     */
    void cleanup();

private:
    // 私有构造函数和析构函数，以实现单例模式
    ShaderManager() = default;
    ~ShaderManager() = default;

    // 使用unordered_map作为缓存，将别名映射到Shader智能指针
    std::unordered_map<std::string, ShaderPtr> m_shaderCache;
};

#endif // SHADER_MANAGER_H