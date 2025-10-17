#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>
#include <cstdint>

class VertexBuffer {
public:
    // 构造函数: 创建并填充VBO
    // data: 指向顶点数据的指针
    // size: 顶点数据的总字节大小
    VertexBuffer(const void* data, uint32_t size);
    ~VertexBuffer();

    // 禁止拷贝, 允许移动
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void bind() const;
    void unbind() const;

private:
    GLuint m_rendererID = 0;
};

#endif // VERTEX_BUFFER_H#pragma once
