#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t count)
    : m_count(count) {
    glGenBuffers(1, &m_rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    if (m_rendererID != 0) {
        glDeleteBuffers(1, &m_rendererID);
    }
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : m_rendererID(other.m_rendererID), m_count(other.m_count) {
    other.m_rendererID = 0;
    other.m_count = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
    if (this != &other) {
        if (m_rendererID != 0) {
            glDeleteBuffers(1, &m_rendererID);
        }
        m_rendererID = other.m_rendererID;
        m_count = other.m_count;
        other.m_rendererID = 0;
        other.m_count = 0;
    }
    return *this;
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}