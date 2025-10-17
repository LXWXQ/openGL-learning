#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include "shader.h"
#include "ShaderManager.h"
#include "Texture.h"
#include <filesystem>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//����Esc�˳�����
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // for ios

    GLFWwindow* window = glfwCreateWindow(800, 600, "GL_Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//�ݴ��ڴ�С��̬�����ӿڴ�С

    float vertices[] = {
      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
    };

    unsigned int indices[] = {
        // ע��������0��ʼ! 
        // ����������(0,1,2,3)���Ƕ�������vertices���±꣬
        // �����������±��������ϳɾ���

        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);//����
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//��
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//��������

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);//����λ������
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//������ɫ����
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//����������������
    glEnableVertexAttribArray(2);


    glBindVertexArray(0);//unbind VAO

    auto Shader = ShaderManager::getInstance().load("test_Shader", "../Shader/learn.vs", "../Shader/learn.fs");
    Texture* texture1 = new Texture("../texture/container.jpg");
    Texture* texture2 = new Texture("../texture/wall.jpg");
    //shader->addTexture(texture1->getTexture(), "texture1");
   // shader->addTexture(texture2->getTexture(), "texture2");
    
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ

    //int vertexColorLocation = glGetUniformLocation(shader->getProgram(), "timeColor");
    //glUniform4f(vertexColorLocation, 0.0f, 0.5f, 0.0f, 1.0f);

    glUniform1i(glGetUniformLocation(Shader->getProgram(), "texture1"), 0);
    glUniform1i(glGetUniformLocation(Shader->getProgram(), "texture2"), 1);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);//������ɫ
        glClear(GL_COLOR_BUFFER_BIT);//��ɫ���塢��Ȼ��塢ģ�建��

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2->getID());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1->getID());

        Shader->use();
        
        glBindVertexArray(VAO);//draw triangles
       // glDrawArrays(GL_TRIANGLES, 0, 3);//ֱ��ʹ��VBO����
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//ֱ��ʹ��EBO����

        glfwSwapBuffers(window);//��������
        glfwPollEvents();//��ȡio��Ϣ(�������)
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();

    return 0;
}