#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, uint32_t size) {
    glGenBuffers(1, &m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    if (m_rendererID != 0) {
        glDeleteBuffers(1, &m_rendererID);
    }
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_rendererID(other.m_rendererID) {
    other.m_rendererID = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
    if (this != &other) {
        if (m_rendererID != 0) {
            glDeleteBuffers(1, &m_rendererID);
        }
        m_rendererID = other.m_rendererID;
        other.m_rendererID = 0;
    }
    return *this;
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}