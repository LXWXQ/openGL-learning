#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>
#include <cstdint>

class VertexBuffer {
public:
    // ���캯��: ���������VBO
    // data: ָ�򶥵����ݵ�ָ��
    // size: �������ݵ����ֽڴ�С
    VertexBuffer(const void* data, uint32_t size);
    ~VertexBuffer();

    // ��ֹ����, �����ƶ�
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
