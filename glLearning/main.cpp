#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include "Shader.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include <filesystem>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "GL_Engine", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

  
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
    std::string backpack = std::string("../Resource/Model/backpack/backpack.obj");
    Model* myModel =new Model(backpack);
    std::string shader = std::string("../Resource/Shader/model");

    auto myShader = ShaderManager::getInstance().load("Model", shader + ".vs", shader + ".fs");
    glm::vec3 pointLightPos = glm::vec3(0, 1, 0);
    // --- 渲染循环 ---
    while (!glfwWindowShouldClose(window))
    {
        // CRITICAL: 处理事件
        glfwPollEvents();

        // 时间计算
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 开始ImGui帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 更新相机
        camera.UpdateFromInput(io, deltaTime);

        // 创建UI窗口
        ImGui::Begin("Engine Controls");
        ImGui::Text("Position: %.2f, %.2f, %.2f", camera.Position.x, camera.Position.y, camera.Position.z);
       
        ImGui::End();

        // --- 渲染 ---
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        // 1. 清屏
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader->use();

        // --- 设置矩阵 ---
        glm::mat4 view = camera.GetViewMatrix();

        float aspectRatio = (float)display_w / (float)display_h;
        camera.projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 100.0f);
    
        // 1. 从一个单位矩阵开始 (代表没有进行任何变换)
        glm::mat4 model = glm::mat4(1.0f);

        // 2. 应用变换 (注意: 顺序通常是 缩放 -> 旋转 -> 位移)
        // a. 把它移动到世界坐标 (0.0f, -1.0f, 0.0f)
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));

        // b. 让它缩小到原来的0.5倍
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

        // c. 让它绕Y轴旋转 (我们可以让它随时间旋转，看起来更酷)
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        myShader->setMat4("u_Projection", camera.projection);
        myShader->setMat4("u_View", view);
        myShader->setMat4("u_Model", model);
        myShader->setMat3("u_NormalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));

        // --- 设置相机位置 ---
        myShader->setVec3("u_ViewPos_World", camera.Position);

        // --- 设置材质 ---
        // (这部分通常由 Mesh::draw 方法处理，它会绑定纹理并设置 sampler uniform)
        // 你只需要在创建Mesh时，确保MeshTexture的类型是 "texture_diffuse" 或 "texture_specular"
        // 你还需要设置光泽度
        myShader->setFloat("u_Shininess", 32.0f); // 这是一个示例值

        // --- 设置方向光 ---
        myShader->setVec3("u_DirLight.direction",glm::vec3(-0.2f,-1.0f,-0.3f));
        myShader->setVec3("u_DirLight.color", glm::vec3(1.0f, 1.0f, 1.0f));

        // --- 设置点光源 ---
        myShader->setVec3("u_PointLight.position", pointLightPos); // 假设这是一个在场景中移动的vec3
        myShader->setVec3("u_PointLight.color", glm::vec3(1.0f, 1.0f, 0.8f));
        myShader->setFloat("u_PointLight.constant", 1.0f);
        myShader->setFloat("u_PointLight.linear", 0.09f);
        myShader->setFloat("u_PointLight.quadratic", 0.032f);

        // --- 绘制模型 ---
        myModel->draw(myShader);

        // 2. 渲染3D场景
        // if (myShader) {
        //     myShader->use();
        //     glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)display_w / (float)display_h, 0.1f, 100.0f);
        //     glm::mat4 view = camera.GetViewMatrix();
        //     myShader->setMat4("projection", projection);
        //     myShader->setMat4("view", view);
        //
        //     glm::mat4 model = glm::mat4(1.0f);
        //     myShader->setMat4("model", model);
        //     myModel.draw(myShader);
        // }

        // 3. 渲染ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // 交换缓冲
        glfwSwapBuffers(window);
    }

    // --- 清理 ---
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}