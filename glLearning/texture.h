#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <memory>

class Texture {
public:
    // ��������
    enum class Type {
        Texture2D,
        TextureCubeMap
    };

    // �����������
    struct Parameters {
        GLenum wrapS = GL_REPEAT;
        GLenum wrapT = GL_REPEAT;
        GLenum wrapR = GL_REPEAT;  // ������������ͼ
        GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
        GLenum magFilter = GL_LINEAR;
        bool generateMipmaps = true;
        bool flipVertically = true;
        bool sRGB = false;  // �Ƿ�ʹ�� sRGB ��ɫ�ռ�

        // �������Թ��� (���֧��)
        float anisotropy = 0.0f;  // 0 ��ʾ��ʹ��
    };

    // ���캯�� - ���ļ�����
    Texture(const std::string& path, Type type = Type::Texture2D);
    Texture(const std::string& path, const Parameters& params, Type type = Type::Texture2D);

    // ���캯�� - ���ڴ����ݴ���
    Texture(int width, int height, GLenum internalFormat, GLenum format,
        GLenum dataType, const void* data = nullptr);

    // ���캯�� - ��������ͼ
    Texture(const std::string faces[6]);  // +X, -X, +Y, -Y, +Z, -Z

    // ��������
    ~Texture();

    // ��ֹ����,�����ƶ�
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    // ������ָ����Ԫ
    void bind(unsigned int unit = 0) const;

    // �������
    void unbind() const;

    // ��ȡ���� ID
    GLuint getID() const { return m_textureID; }

    // ��������Ƿ���Ч
    bool isValid() const { return m_textureID != 0; }

    // ��ȡ������Ϣ
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getChannels() const { return m_channels; }
    Type getType() const { return m_type; }

    // ������������
    void updateData(int xOffset, int yOffset, int width, int height,
        GLenum format, GLenum dataType, const void* data);

    // ���¼������� (������)
    bool reload();

    // ���� Mipmap
    void generateMipmaps();

    // �����������
    void setParameter(GLenum param, GLint value);
    void setParameter(GLenum param, GLfloat value);

private:
    GLuint m_textureID = 0;
    Type m_type = Type::Texture2D;
    std::string m_path;  // ����·����������
    Parameters m_params;

    // ������Ϣ
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;

    // �ڲ���������
    void loadFromFile(const std::string& path, const Parameters& params);
    void loadCubemap(const std::string faces[6]);
    void setupTextureParameters(const Parameters& params);
    GLenum getInternalFormat(int channels, bool sRGB) const;
    GLenum getFormat(int channels) const;
    GLenum getTextureTarget() const;
};

// ����ָ�����ͱ���
using TexturePtr = std::shared_ptr<Texture>;
using TextureUniquePtr = std::unique_ptr<Texture>;

#endif // TEXTURE_H