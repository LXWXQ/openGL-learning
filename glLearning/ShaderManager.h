#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "Shader.h" // ������д��Shaderͷ�ļ�
#include <string>
#include <unordered_map>
#include <memory>

// ShaderManager��һ�������࣬������ء��洢���ṩ��Shader����ķ���
class ShaderManager {
public:
    // ��ȡ����ʵ���ľ�̬����
    static ShaderManager& getInstance();

    // ��ֹ�����͸�ֵ����ά��������Ψһ��
    ShaderManager(const ShaderManager&) = delete;
    void operator=(const ShaderManager&) = delete;

    /**
     * @brief ����(���ȡ�Ѽ��ص�)һ����ɫ������.
     * @param name �����ڹ�������Ψһ��ʶ����ɫ���ı���.
     * @param vertexPath ������ɫ���ļ�·��.
     * @param fragmentPath Ƭ����ɫ���ļ�·��.
     * @return ����һ��ָ��Shader�Ĺ���ָ��. �������ʧ���򷵻�nullptr.
     */
    ShaderPtr load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

    /**
     * @brief ����(���ȡ�Ѽ��ص�)һ����������ɫ������ɫ������.
     * @param name �����ڹ�������Ψһ��ʶ����ɫ���ı���.
     * @param vertexPath ������ɫ���ļ�·��.
     * @param fragmentPath Ƭ����ɫ���ļ�·��.
     * @param geometryPath ������ɫ���ļ�·��.
     * @return ����һ��ָ��Shader�Ĺ���ָ��. �������ʧ���򷵻�nullptr.
     */
    ShaderPtr load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);

    /**
     * @brief ͨ��������ȡһ���Ѿ����ص���ɫ��.
     * @param name ��ɫ���ı���.
     * @return ����һ��ָ��Shader�Ĺ���ָ��. ���δ�ҵ��򷵻�nullptr.
     */
    ShaderPtr get(const std::string& name) const;

    /**
     * @brief ���¼���������ע�����ɫ��. ����ʵʱ����.
     */
    void reloadAll();

    /**
     * @brief ���������Ѽ��ص���ɫ����Դ.
     */
    void cleanup();

private:
    // ˽�й��캯����������������ʵ�ֵ���ģʽ
    ShaderManager() = default;
    ~ShaderManager() = default;

    // ʹ��unordered_map��Ϊ���棬������ӳ�䵽Shader����ָ��
    std::unordered_map<std::string, ShaderPtr> m_shaderCache;
};

#endif // SHADER_MANAGER_H