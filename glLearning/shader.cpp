#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// ===== 构造函数 =====
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    initFromFiles(vertexPath, fragmentPath);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath,
    const std::string& geometryPath) {
    initFromFiles(vertexPath, fragmentPath, geometryPath);
}

// ===== 析构函数 =====
Shader::~Shader() {
    if (m_programID != 0) {
        glDeleteProgram(m_programID);
    }
}

// ===== 移动构造和赋值 =====
Shader::Shader(Shader&& other) noexcept
    : m_programID(other.m_programID),
    m_vertexPath(std::move(other.m_vertexPath)),
    m_fragmentPath(std::move(other.m_fragmentPath)),
    m_geometryPath(std::move(other.m_geometryPath)),
    m_uniformLocationCache(std::move(other.m_uniformLocationCache)) {
    other.m_programID = 0; // 防止被释放
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        // 释放当前资源
        if (m_programID != 0) {
            glDeleteProgram(m_programID);
        }

        // 移动资源
        m_programID = other.m_programID;
        m_vertexPath = std::move(other.m_vertexPath);
        m_fragmentPath = std::move(other.m_fragmentPath);
        m_geometryPath = std::move(other.m_geometryPath);
        m_uniformLocationCache = std::move(other.m_uniformLocationCache);

        other.m_programID = 0;
    }
    return *this;
}

// ===== 使用着色器 =====
void Shader::use() const {
    if (m_programID != 0) {
        glUseProgram(m_programID);
    }
    else {
        std::cerr << "ERROR::SHADER: Attempting to use invalid shader program!" << std::endl;
    }
}

// ===== Uniform设置函数 =====
void Shader::setBool(const std::string& name, bool value) {
    glUniform1i(getUniformLocation(name), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) {
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setIntArray(const std::string& name, const int* values, size_t count) {
    glUniform1iv(getUniformLocation(name), static_cast<GLsizei>(count), values);
}

// ===== 热重载功能 =====
bool Shader::reload() {
    std::cout << "Reloading shader..." << std::endl;

    // 保存旧的程序ID
    GLuint oldProgram = m_programID;
    m_programID = 0;

    // 清除缓存
    m_uniformLocationCache.clear();

    try {
        // 重新加载
        initFromFiles(m_vertexPath, m_fragmentPath, m_geometryPath);

        // 删除旧程序
        if (oldProgram != 0) {
            glDeleteProgram(oldProgram);
        }

        std::cout << "Shader reloaded successfully!" << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to reload shader: " << e.what() << std::endl;
        // 恢复旧程序
        m_programID = oldProgram;
        return false;
    }
}

void Shader::clearUniformCache() {
    m_uniformLocationCache.clear();
}

// ===== 私有辅助函数 =====
void Shader::initFromFiles(const std::string& vertexPath,
    const std::string& fragmentPath,
    const std::string& geometryPath) {
    // 保存路径
    m_vertexPath = vertexPath;
    m_fragmentPath = fragmentPath;
    m_geometryPath = geometryPath;

    // 读取文件
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);
    std::string geometryCode;

    if (!geometryPath.empty()) {
        geometryCode = readFile(geometryPath);
    }

    // 编译着色器
    GLuint vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER, "VERTEX");
    GLuint fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER, "FRAGMENT");
    GLuint geometryShader = 0;

    if (!geometryPath.empty()) {
        geometryShader = compileShader(geometryCode, GL_GEOMETRY_SHADER, "GEOMETRY");
    }

    // 链接程序
    m_programID = linkProgram(vertexShader, fragmentShader, geometryShader);

    // 清理着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryShader != 0) {
        glDeleteShader(geometryShader);
    }
}

std::string Shader::readFile(const std::string& filepath) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(filepath);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        return stream.str();
    }
    catch (const std::ifstream::failure& e) {
        std::string error = "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " + filepath + "\n" + e.what();
        throw std::runtime_error(error);
    }
}

GLuint Shader::compileShader(const std::string& source, GLenum type, const std::string& typeName) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, typeName);
    return shader;
}

GLuint Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    if (geometryShader != 0) {
        glAttachShader(program, geometryShader);
    }
    glLinkProgram(program);
    checkLinkErrors(program);
    return program;
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::string error = "ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" +
            infoLog + "\n -- --------------------------------------------------- --";
        throw std::runtime_error(error);
    }
}

void Shader::checkLinkErrors(GLuint program) {
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::string error = "ERROR::PROGRAM_LINKING_ERROR\n" +
            std::string(infoLog) +
            "\n -- --------------------------------------------------- --";
        throw std::runtime_error(error);
    }
}

GLint Shader::getUniformLocation(const std::string& name) const {
    // 查找缓存
    auto it = m_uniformLocationCache.find(name);
    if (it != m_uniformLocationCache.end()) {
        return it->second;
    }

    // 查询OpenGL
    GLint location = glGetUniformLocation(m_programID, name.c_str());

    // 警告:未找到uniform
    if (location == -1) {
        std::cerr << "WARNING::SHADER: Uniform '" << name << "' not found in shader!" << std::endl;
    }

    // 缓存结果
    m_uniformLocationCache[name] = location;
    return location;
}