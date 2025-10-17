#include "VertexArray.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray() {
    if (m_rendererID != 0) {
        glDeleteVertexArrays(1, &m_rendererID);
    }
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_rendererID(other.m_rendererID) {
    other.m_rendererID = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
    if (this != &other) {
        if (m_rendererID != 0) {
            glDeleteVertexArrays(1, &m_rendererID);
        }
        m_rendererID = other.m_rendererID;
        other.m_rendererID = 0;
    }
    return *this;
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    bind();
    vb.bind();
    const auto& elements = layout.getElements();
    uintptr_t offset = 0;
    for (GLuint i = 0; i < elements.size(); ++i) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized,
            layout.getStride(), (const void*)offset);
        offset += element.count * VertexAttribute::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const {
    glBindVertexArray(m_rendererID);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}


/*ʹ��demo

#include <memory> // for std::unique_ptr
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h" // �������Ѿ�����Shader��

void main() {
    // ... (���ں�GLAD��ʼ��) ...

    // 1. ���嶥������ (λ��x, y, z + ��ɫr, g, b)
    float vertices[] = {
        // λ��           // ��ɫ
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // ���½�
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // ���½�
         0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // ���Ͻ�
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f  // ���Ͻ�
    };

    // 2. ������������ (������������������ɾ���)
    uint32_t indices[] = {
        0, 1, 2, // ��һ��������
        2, 3, 0  // �ڶ���������
    };

    // 3. �����������Ͷ���������� (ʹ������ָ�������������)
    auto vao = std::make_unique<VertexArray>();
    auto vbo = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    auto ibo = std::make_unique<IndexBuffer>(indices, 6);

    // 4. ���嶥�㲼��
    VertexBufferLayout layout;
    layout.push<float>(3); // ���һ�� vec3 ��λ������
    layout.push<float>(3); // ���һ�� vec3 ����ɫ����

    // 5. �� VBO ���䲼����ӵ� VAO
    vao->addBuffer(*vbo, layout);

    // 6. ������ɫ��
    auto shader = std::make_unique<Shader>("path/to/shader.vs", "path/to/shader.fs");

    // 7. ����Ⱦѭ���л���
    while (!glfwWindowShouldClose(window)) {
        // ... ���� ...

        shader->use();
        vao->bind();
        ibo->bind(); // VAO ����"��ס"IBO�İ󶨣������ڻ���ǰ��

        glDrawElements(GL_TRIANGLES, ibo->getCount(), GL_UNSIGNED_INT, nullptr);

        // ... �������� ...
    }

    // vao, vbo, ibo, shader ���� main ��������ʱ�Զ����٣����ͷ�GPU��Դ
}
*/