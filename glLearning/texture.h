#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <memory>

class Texture {
public:
    // 纹理类型
    enum class Type {
        Texture2D,
        TextureCubeMap
    };

    // 纹理参数配置
    struct Parameters {
        GLenum wrapS = GL_REPEAT;
        GLenum wrapT = GL_REPEAT;
        GLenum wrapR = GL_REPEAT;  // 用于立方体贴图
        GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
        GLenum magFilter = GL_LINEAR;
        bool generateMipmaps = true;
        bool flipVertically = true;
        bool sRGB = false;  // 是否使用 sRGB 颜色空间

        // 各向异性过滤 (如果支持)
        float anisotropy = 0.0f;  // 0 表示不使用
    };

    // 构造函数 - 从文件加载
    Texture(const std::string& path, Type type = Type::Texture2D);
    Texture(const std::string& path, const Parameters& params, Type type = Type::Texture2D);

    // 构造函数 - 从内存数据创建
    Texture(int width, int height, GLenum internalFormat, GLenum format,
        GLenum dataType, const void* data = nullptr);

    // 构造函数 - 立方体贴图
    Texture(const std::string faces[6]);  // +X, -X, +Y, -Y, +Z, -Z

    // 析构函数
    ~Texture();

    // 禁止拷贝,允许移动
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    // 绑定纹理到指定单元
    void bind(unsigned int unit = 0) const;

    // 解绑纹理
    void unbind() const;

    // 获取纹理 ID
    GLuint getID() const { return m_textureID; }

    // 检查纹理是否有效
    bool isValid() const { return m_textureID != 0; }

    // 获取纹理信息
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getChannels() const { return m_channels; }
    Type getType() const { return m_type; }

    // 更新纹理数据
    void updateData(int xOffset, int yOffset, int width, int height,
        GLenum format, GLenum dataType, const void* data);

    // 重新加载纹理 (热重载)
    bool reload();

    // 生成 Mipmap
    void generateMipmaps();

    // 设置纹理参数
    void setParameter(GLenum param, GLint value);
    void setParameter(GLenum param, GLfloat value);

private:
    GLuint m_textureID = 0;
    Type m_type = Type::Texture2D;
    std::string m_path;  // 保存路径用于重载
    Parameters m_params;

    // 纹理信息
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;

    // 内部辅助函数
    void loadFromFile(const std::string& path, const Parameters& params);
    void loadCubemap(const std::string faces[6]);
    void setupTextureParameters(const Parameters& params);
    GLenum getInternalFormat(int channels, bool sRGB) const;
    GLenum getFormat(int channels) const;
    GLenum getTextureTarget() const;
};

// 智能指针类型别名
using TexturePtr = std::shared_ptr<Texture>;
using TextureUniquePtr = std::unique_ptr<Texture>;

#endif // TEXTURE_H