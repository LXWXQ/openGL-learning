#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <glad/glad.h>
#include <cstdint>

class IndexBuffer {
public:
    // 构造函数: 创建并填充IBO
    // indices: 指向索引数据的指针 (必须是无符号整数)
    // count: 索引的数量
    IndexBuffer(const uint32_t* indices, uint32_t count);
    ~IndexBuffer();

    // 禁止拷贝, 允许移动
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