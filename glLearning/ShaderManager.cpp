#include "ShaderManager.h"
#include <iostream>

// ��ȡ����ʵ�� (Meyers' Singleton - �̰߳�ȫ C++11���Ժ�)
ShaderManager& ShaderManager::getInstance() {
    static ShaderManager instance;
    return instance;
}

// ������ɫ�� (�޼�����ɫ���汾)
ShaderPtr ShaderManager::load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
    // ���ȼ�黺�����Ƿ��Ѵ���
    if (m_shaderCache.count(name)) {
        return m_shaderCache[name];
    }

    std::cout << "SHADER_MANAGER: Loading shader '" << name << "' from files..." << std::endl;
    try {
        // �����µ�Shader����
        auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);

        // ���Shader�Ƿ�ɹ�����
        if (shader->isValid()) {
            m_shaderCache[name] = shader;
            return shader;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "SHADER_MANAGER: Failed to load shader '" << name << "'.\n" << e.what() << std::endl;
    }

    return nullptr; // ����ʧ��
}

// ������ɫ�� (��������ɫ���汾)
ShaderPtr ShaderManager::load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
    if (m_shaderCache.count(name)) {
        return m_shaderCache[name];
    }

    std::cout << "SHADER_MANAGER: Loading shader '" << name << "' with geometry shader from files..." << std::endl;
    try {
        auto shader = std::make_shared<Shader>(vertexPath, fragmentPath, geometryPath);
        if (shader->isValid()) {
            m_shaderCache[name] = shader;
            return shader;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "SHADER_MANAGER: Failed to load shader '" << name << "'.\n" << e.what() << std::endl;
    }

    return nullptr;
}


// ͨ��������ȡ��ɫ��
ShaderPtr ShaderManager::get(const std::string& name) const {
    auto it = m_shaderCache.find(name);
    if (it != m_shaderCache.end()) {
        return it->second;
    }

    std::cerr << "WARNING::SHADER_MANAGER: Shader '" << name << "' not found!" << std::endl;
    return nullptr;
}

// ���¼���������ɫ��
void ShaderManager::reloadAll() {
    std::cout << "\n--- RELOADING ALL SHADERS ---" << std::endl;
    for (auto& [name, shader] : m_shaderCache) {
        std::cout << "Reloading '" << name << "'...";
        if (shader->reload()) {
            std::cout << " SUCCESS" << std::endl;
        }
        else {
            std::cout << " FAILED" << std::endl;
        }
    }
    std::cout << "--- RELOAD COMPLETE ---\n" << std::endl;
}

// ����������Դ
void ShaderManager::cleanup() {
    std::cout << "SHADER_MANAGER: Cleaning up all shaders..." << std::endl;
    // ��map�����ʱ, shared_ptr�����ü����ήΪ0, �Զ�����Shader����������
    m_shaderCache.clear();
}