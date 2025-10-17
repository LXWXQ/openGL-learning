#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <memory>

class Shader {
public:
    // ���캯�� - ֧�ֿ�ѡ�ļ�����ɫ��
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    Shader(const std::string& vertexPath, const std::string& fragmentPath,
        const std::string& geometryPath);

    // ��������
    ~Shader();

    // ��ֹ����,�����ƶ�
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    // ʹ����ɫ��
    void use() const;

    // ��ȡ����ID
    GLuint getProgram() const { return m_programID; }

    // �����ɫ���Ƿ���Ч
    bool isValid() const { return m_programID != 0; }

    // Uniform���ú��� - �������Ż�
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat3(const std::string& name, const glm::mat3& mat);
    void setMat4(const std::string& name, const glm::mat4& mat);

    // �������ö��int (��������Ԫ)
    void setIntArray(const std::string& name, const int* values, size_t count);

    // �����ع��� (����ʱ�ǳ�����)
    bool reload();

    // ���Uniform����
    void clearUniformCache();

private:
    GLuint m_programID = 0;

    // ����·������������
    std::string m_vertexPath;
    std::string m_fragmentPath;
    std::string m_geometryPath;

    // Uniform location����,�����ظ���ѯ
    mutable std::unordered_map<std::string, GLint> m_uniformLocationCache;

    // �ڲ���������
    GLuint compileShader(const std::string& source, GLenum type, const std::string& typeName);
    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = 0);
    std::string readFile(const std::string& filepath);
    void checkCompileErrors(GLuint shader, const std::string& type);
    void checkLinkErrors(GLuint program);

    // ��ȡUniform location (������)
    GLint getUniformLocation(const std::string& name) const;

    // ��ʼ������
    void initFromFiles(const std::string& vertexPath,
        const std::string& fragmentPath,
        const std::string& geometryPath = "");
};

// ����ָ�����ͱ���
using ShaderPtr = std::shared_ptr<Shader>;
using ShaderUniquePtr = std::unique_ptr<Shader>;

#endif // SHADER_H