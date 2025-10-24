#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h" // 包含ImGui头文件

// 定义相机移动的几个可能方向
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// 默认相机参数值
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f; // FOV

class Camera {
public:
    // --- 相机属性 ---
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // --- 欧拉角 ---
    float Yaw;
    float Pitch;

    // --- 相机选项 ---
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom; // Field of View (FOV)
    glm::mat4 projection = glm::mat4(1.0);//暂时放这里
    // --- 构造函数 ---
    // 使用向量构造
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW, float pitch = PITCH);

    // --- 公有方法 ---

    // 返回使用欧拉角和LookAt矩阵计算出的视图矩阵
    glm::mat4 GetViewMatrix() const;

    void UpdateFromInput(const ImGuiIO& io, float deltaTime);


private:
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    // 更新相机向量 (不变)
    void updateCameraVectors();
};

#endif // CAMERA_H