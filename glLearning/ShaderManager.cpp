#include "ShaderManager.h"
#include <iostream>

// 获取单例实例 (Meyers' Singleton - 线程安全 C++11及以后)
ShaderManager& ShaderManager::getInstance() {
    static ShaderManager instance;
    return instance;
}

// 加载着色器 (无几何着色器版本)
ShaderPtr ShaderManager::load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
    // 首先检查缓存中是否已存在
    if (m_shaderCache.count(name)) {
        return m_shaderCache[name];
    }

    std::cout << "SHADER_MANAGER: Loading shader '" << name << "' from files..." << std::endl;
    try {
        // 创建新的Shader对象
        auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);

        // 检查Shader是否成功创建
        if (shader->isValid()) {
            m_shaderCache[name] = shader;
            return shader;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "SHADER_MANAGER: Failed to load shader '" << name << "'.\n" << e.what() << std::endl;
    }

    return nullptr; // 加载失败
}

// 加载着色器 (带几何着色器版本)
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


// 通过别名获取着色器
ShaderPtr ShaderManager::get(const std::string& name) const {
    auto it = m_shaderCache.find(name);
    if (it != m_shaderCache.end()) {
        return it->second;
    }

    std::cerr << "WARNING::SHADER_MANAGER: Shader '" << name << "' not found!" << std::endl;
    return nullptr;
}

// 重新加载所有着色器
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

// 清理所有资源
void ShaderManager::cleanup() {
    std::cout << "SHADER_MANAGER: Cleaning up all shaders..." << std::endl;
    // 当map被清空时, shared_ptr的引用计数会降为0, 自动调用Shader的析构函数
    m_shaderCache.clear();
}