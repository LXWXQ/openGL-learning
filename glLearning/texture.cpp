#include "Texture.h"
#include <iostream>

// 注意: STB_IMAGE_IMPLEMENTATION 应该只在一个 .cpp 文件中定义
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ===== 构造函数 =====
Texture::Texture(const std::string& path, Type type)
    : m_type(type), m_path(path) {
    loadFromFile(path, Parameters());
}

Texture::Texture(const std::string& path, const Parameters& params, Type type)
    : m_type(type), m_path(path), m_params(params) {
    loadFromFile(path, params);
}

Texture::Texture(int width, int height, GLenum internalFormat, GLenum format,
    GLenum dataType, const void* data)
    : m_width(width), m_height(height) {

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0,
        format, dataType, data);

    // 默认参数
    setupTextureParameters(Parameters());

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string faces[6])
    : m_type(Type::TextureCubeMap) {
    loadCubemap(faces);
}

// ===== 析构函数 =====
Texture::~Texture() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
    }
}

// ===== 移动构造和赋值 =====
Texture::Texture(Texture&& other) noexcept
    : m_textureID(other.m_textureID),
    m_type(other.m_type),
    m_path(std::move(other.m_path)),
    m_params(other.m_params),
    m_width(other.m_width),
    m_height(other.m_height),
    m_channels(other.m_channels) {
    other.m_textureID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        // 释放当前资源
        if (m_textureID != 0) {
            glDeleteTextures(1, &m_textureID);
        }

        // 移动资源
        m_textureID = other.m_textureID;
        m_type = other.m_type;
        m_path = std::move(other.m_path);
        m_params = other.m_params;
        m_width = other.m_width;
        m_height = other.m_height;
        m_channels = other.m_channels;

        other.m_textureID = 0;
    }
    return *this;
}

// ===== 绑定和解绑 =====
void Texture::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(getTextureTarget(), m_textureID);
}

void Texture::unbind() const {
    glBindTexture(getTextureTarget(), 0);
}

// ===== 更新纹理数据 =====
void Texture::updateData(int xOffset, int yOffset, int width, int height,
    GLenum format, GLenum dataType, const void* data) {
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height,
        format, dataType, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// ===== 重新加载 =====
bool Texture::reload() {
    if (m_path.empty()) {
        std::cerr << "ERROR::TEXTURE: Cannot reload texture with no path" << std::endl;
        return false;
    }

    std::cout << "Reloading texture: " << m_path << std::endl;

    // 删除旧纹理
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
    }

    try {
        if (m_type == Type::TextureCubeMap) {
            // 对于立方体贴图,需要保存所有面的路径
            std::cerr << "ERROR::TEXTURE: Cubemap reload not yet implemented" << std::endl;
            return false;
        }
        else {
            loadFromFile(m_path, m_params);
        }
        std::cout << "Texture reloaded successfully!" << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to reload texture: " << e.what() << std::endl;
        return false;
    }
}

// ===== 生成 Mipmaps =====
void Texture::generateMipmaps() {
    glBindTexture(getTextureTarget(), m_textureID);
    glGenerateMipmap(getTextureTarget());
    glBindTexture(getTextureTarget(), 0);
}

// ===== 设置纹理参数 =====
void Texture::setParameter(GLenum param, GLint value) {
    glBindTexture(getTextureTarget(), m_textureID);
    glTexParameteri(getTextureTarget(), param, value);
    glBindTexture(getTextureTarget(), 0);
}

void Texture::setParameter(GLenum param, GLfloat value) {
    glBindTexture(getTextureTarget(), m_textureID);
    glTexParameterf(getTextureTarget(), param, value);
    glBindTexture(getTextureTarget(), 0);
}

// ===== 私有辅助函数 =====
void Texture::loadFromFile(const std::string& path, const Parameters& params) {
    // 设置 stb_image 翻转
    stbi_set_flip_vertically_on_load(params.flipVertically);

    // 加载图片
    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);

    if (!data) {
        std::string error = "ERROR::TEXTURE: Failed to load texture: " + path;
        throw std::runtime_error(error);
    }

    // 生成纹理
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // 根据通道数确定格式
    GLenum internalFormat = getInternalFormat(m_channels, params.sRGB);
    GLenum format = getFormat(m_channels);

    // 上传纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0,
        format, GL_UNSIGNED_BYTE, data);

    // 生成 Mipmaps
    if (params.generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // 设置纹理参数
    setupTextureParameters(params);

    glBindTexture(GL_TEXTURE_2D, 0);

    // 释放图片内存
    stbi_image_free(data);

    std::cout << "SUCCESS::TEXTURE: Loaded texture '" << path << "' ("
        << m_width << "x" << m_height << ", " << m_channels << " channels)" << std::endl;
}

void Texture::loadCubemap(const std::string faces[6]) {
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

    stbi_set_flip_vertically_on_load(false);  // 立方体贴图通常不翻转

    for (unsigned int i = 0; i < 6; i++) {
        int width, height, channels;
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);

        if (data) {
            GLenum format = getFormat(channels);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
                width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);

            // 保存第一个面的信息
            if (i == 0) {
                m_width = width;
                m_height = height;
                m_channels = channels;
            }
        }
        else {
            std::cerr << "ERROR::TEXTURE: Cubemap texture failed to load at path: "
                << faces[i] << std::endl;
            stbi_image_free(data);
            throw std::runtime_error("Failed to load cubemap face");
        }
    }

    // 立方体贴图参数
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    std::cout << "SUCCESS::TEXTURE: Loaded cubemap texture" << std::endl;
}

void Texture::setupTextureParameters(const Parameters& params) {
    GLenum target = getTextureTarget();

    glTexParameteri(target, GL_TEXTURE_WRAP_S, params.wrapS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, params.wrapT);

    if (m_type == Type::TextureCubeMap) {
        glTexParameteri(target, GL_TEXTURE_WRAP_R, params.wrapR);
    }

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, params.minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, params.magFilter);

    // 各向异性过滤 (如果支持)
    if (params.anisotropy > 0.0f) 
    {
        GLfloat maxAnisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        float anisotropy = std::min(params.anisotropy, maxAnisotropy);
        glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
    }
}

GLenum Texture::getInternalFormat(int channels, bool sRGB) const {
    switch (channels) {
    case 1: return GL_RED;
    case 2: return GL_RG;
    case 3: return sRGB ? GL_SRGB : GL_RGB;
    case 4: return sRGB ? GL_SRGB_ALPHA : GL_RGBA;
    default:
        std::cerr << "ERROR::TEXTURE: Unsupported number of channels: " << channels << std::endl;
        return GL_RGB;
    }
}

GLenum Texture::getFormat(int channels) const {
    switch (channels) {
    case 1: return GL_RED;
    case 2: return GL_RG;
    case 3: return GL_RGB;
    case 4: return GL_RGBA;
    default:
        std::cerr << "ERROR::TEXTURE: Unsupported number of channels: " << channels << std::endl;
        return GL_RGB;
    }
}

GLenum Texture::getTextureTarget() const {
    return m_type == Type::TextureCubeMap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
}