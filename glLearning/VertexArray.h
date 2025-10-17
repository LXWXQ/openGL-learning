#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include <vector>
#include <glad/glad.h>
#include <stdexcept>

// ���������������ԵĽṹ
struct VertexAttribute {
    GLuint type;
    GLuint count;
    GLboolean normalized;

    static GLuint getSizeOfType(GLuint type) {
        switch (type) {
        case GL_FLOAT:         return sizeof(GLfloat);
        case GL_UNSIGNED_INT:  return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        }
        // ���Ի��׳�����
        return 0;
    }
};

// �����������㻺�岼�ֵ���
class VertexBufferLayout {
public:
    VertexBufferLayout() : m_stride(0) {}

    // ģ�庯������������µ�����
    template<typename T>
    void push(GLuint count) {
        throw std::runtime_error("Unsupported type for VertexBufferLayout!");
    }

    const std::vector<VertexAttribute>& getElements() const { return m_elements; }
    GLuint getStride() const { return m_stride; }

private:
    std::vector<VertexAttribute> m_elements;
    GLuint m_stride;
};

// ģ���ػ�
template<> inline void VertexBufferLayout::push<float>(GLuint count) {
    m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_stride += count * VertexAttribute::getSizeOfType(GL_FLOAT);
}

template<> inline void VertexBufferLayout::push<unsigned int>(GLuint count) {
    m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_stride += count * VertexAttribute::getSizeOfType(GL_UNSIGNED_INT);
}

// ��װ����������� (VAO) ����
class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    // ��ֹ����, �����ƶ�
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    // �� VBO ���䲼����ӵ� VAO
    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void bind() const;
    void unbind() const;

private:
    GLuint m_rendererID = 0;
};

#endif // VERTEX_ARRAY_H