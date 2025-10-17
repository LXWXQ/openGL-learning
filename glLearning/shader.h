#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <memory>

class Shader {
public:
    // 构造函数 - 支持可选的几何着色器
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    Shader(const std::string& vertexPath, const std::string& fragmentPath,
        const std::string& geometryPath);

    // 析构函数
    ~Shader();

    // 禁止拷贝,允许移动
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    // 使用着色器
    void use() const;

    // 获取程序ID
    GLuint getProgram() const { return m_programID; }

    // 检查着色器是否有效
    bool isValid() const { return m_programID != 0; }

    // Uniform设置函数 - 带缓存优化
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat3(const std::string& name, const glm::mat3& mat);
    void setMat4(const std::string& name, const glm::mat4& mat);

    // 批量设置多个int (用于纹理单元)
    void setIntArray(const std::string& name, const int* values, size_t count);

    // 热重载功能 (调试时非常有用)
    bool reload();

    // 清除Uniform缓存
    void clearUniformCache();

private:
    GLuint m_programID = 0;

    // 保存路径用于热重载
    std::string m_vertexPath;
    std::string m_fragmentPath;
    std::string m_geometryPath;

    // Uniform location缓存,避免重复查询
    mutable std::unordered_map<std::string, GLint> m_uniformLocationCache;

    // 内部辅助函数
    GLuint compileShader(const std::string& source, GLenum type, const std::string& typeName);
    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = 0);
    std::string readFile(const std::string& filepath);
    void checkCompileErrors(GLuint shader, const std::string& type);
    void checkLinkErrors(GLuint program);

    // 获取Uniform location (带缓存)
    GLint getUniformLocation(const std::string& name) const;

    // 初始化函数
    void initFromFiles(const std::string& vertexPath,
        const std::string& fragmentPath,
        const std::string& geometryPath = "");
};

// 智能指针类型别名
using ShaderPtr = std::shared_ptr<Shader>;
using ShaderUniquePtr = std::unique_ptr<Shader>;

#endif // SHADER_H