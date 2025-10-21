#include "Camera.h"

// --- 构造函数 ---

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}


glm::mat4 Camera::GetViewMatrix() const {
    // 使用 glm::lookAt 计算视图矩阵
    // 参数: 相机位置, 目标位置, 上向量
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::UpdateFromInput(const ImGuiIO& io, float deltaTime) {
    // 关键检查：如果ImGui想要捕获鼠标或键盘，相机就不做任何响应
    if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
        return;
    }

    // --- 鼠标观察 ---
    // 通常我们希望按住右键才移动视角
    if (io.MouseDown[ImGuiMouseButton_Right]) {
        ProcessMouseMovement(io.MouseDelta.x, io.MouseDelta.y);
    }

    // --- 键盘移动 ---
    
    if (ImGui::IsKeyDown(ImGuiKey_W)) ProcessKeyboard(FORWARD, deltaTime);
    if (ImGui::IsKeyDown(ImGuiKey_S)) ProcessKeyboard(BACKWARD, deltaTime);
    if (ImGui::IsKeyDown(ImGuiKey_A)) ProcessKeyboard(LEFT, deltaTime);
    if (ImGui::IsKeyDown(ImGuiKey_D)) ProcessKeyboard(RIGHT, deltaTime);
    if (ImGui::IsKeyDown(ImGuiKey_Space)) ProcessKeyboard(UP, deltaTime);
    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) ProcessKeyboard(DOWN, deltaTime);

    // --- 鼠标滚轮缩放 ---
    if (io.MouseWheel != 0.0f) 
    {
        ProcessMouseScroll(io.MouseWheel);
    }
}


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UP)
        Position += WorldUp * velocity; // 使用世界坐标的上方向来升降
    if (direction == DOWN)
        Position -= WorldUp * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch ) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // 限制俯仰角，避免万向节死锁问题导致屏幕翻转
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // 更新 Front, Right, Up 向量
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

// --- 私有方法实现 ---

void Camera::updateCameraVectors() {
    // 根据最新的偏航角(Yaw)和俯仰角(Pitch)计算新的 Front 向量
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // 重新计算 Right 和 Up 向量
    // 注意: 使用 WorldUp 来计算 Right 向量，以避免因相机滚动产生的问题
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}