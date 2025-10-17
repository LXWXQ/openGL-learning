#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include <vector>
#include <glad/glad.h>
#include <stdexcept>

// 描述单个顶点属性的结构
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
        // 断言或抛出错误
        return 0;
    }
};

// 描述整个顶点缓冲布局的类
class VertexBufferLayout {
public:
    VertexBufferLayout() : m_stride(0) {}

    // 模板函数，用于添加新的属性
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

// 模板特化
template<> inline void VertexBufferLayout::push<float>(GLuint count) {
    m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_stride += count * VertexAttribute::getSizeOfType(GL_FLOAT);
}

template<> inline void VertexBufferLayout::push<unsigned int>(GLuint count) {
    m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_stride += count * VertexAttribute::getSizeOfType(GL_UNSIGNED_INT);
}

// 封装顶点数组对象 (VAO) 的类
class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    // 禁止拷贝, 允许移动
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    // 将 VBO 和其布局添加到 VAO
    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void bind() const;
    void unbind() const;

private:
    GLuint m_rendererID = 0;
};

#endif // VERTEX_ARRAY_H