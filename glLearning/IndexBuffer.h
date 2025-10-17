#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <glad/glad.h>
#include <cstdint>

class IndexBuffer {
public:
    // ���캯��: ���������IBO
    // indices: ָ���������ݵ�ָ�� (�������޷�������)
    // count: ����������
    IndexBuffer(const uint32_t* indices, uint32_t count);
    ~IndexBuffer();

    // ��ֹ����, �����ƶ�
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;
    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;

    void bind() const;
    void unbind() const;

    uint32_t getCount() const { return m_count; }

private:
    GLuint m_rendererID = 0;
    uint32_t m_count;
};

#endif // INDEX_BUFFER_H