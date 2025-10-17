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


/*使用demo

#include <memory> // for std::unique_ptr
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h" // 假设你已经有了Shader类

void main() {
    // ... (窗口和GLAD初始化) ...

    // 1. 定义顶点数据 (位置x, y, z + 颜色r, g, b)
    float vertices[] = {
        // 位置           // 颜色
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // 左下角
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // 右下角
         0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // 右上角
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f  // 左上角
    };

    // 2. 定义索引数据 (绘制两个三角形来组成矩形)
    uint32_t indices[] = {
        0, 1, 2, // 第一个三角形
        2, 3, 0  // 第二个三角形
    };

    // 3. 创建缓冲对象和顶点数组对象 (使用智能指针管理生命周期)
    auto vao = std::make_unique<VertexArray>();
    auto vbo = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    auto ibo = std::make_unique<IndexBuffer>(indices, 6);

    // 4. 定义顶点布局
    VertexBufferLayout layout;
    layout.push<float>(3); // 添加一个 vec3 的位置属性
    layout.push<float>(3); // 添加一个 vec3 的颜色属性

    // 5. 将 VBO 和其布局添加到 VAO
    vao->addBuffer(*vbo, layout);

    // 6. 加载着色器
    auto shader = std::make_unique<Shader>("path/to/shader.vs", "path/to/shader.fs");

    // 7. 在渲染循环中绘制
    while (!glfwWindowShouldClose(window)) {
        // ... 清屏 ...

        shader->use();
        vao->bind();
        ibo->bind(); // VAO 不会"记住"IBO的绑定，所以在绘制前绑定

        glDrawElements(GL_TRIANGLES, ibo->getCount(), GL_UNSIGNED_INT, nullptr);

        // ... 交换缓冲 ...
    }

    // vao, vbo, ibo, shader 会在 main 函数结束时自动销毁，并释放GPU资源
}
*/