// 需要一个数学库来处理向量和矩阵，这里以GLM为例
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

// TransformComponent.h
struct Transform {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation_Aixs = glm::vec3(0.0f, 1.0f, 0.0f); // W, X, Y, Z
    float rotation_angle = 0.0f;
    glm::vec3 scale = glm::vec3(1.0f);

    // 计算并返回该变换的模型矩阵
    glm::mat4 getModelMatrix() const 
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model,glm::radians(rotation_angle), rotation_Aixs);
        model = glm::scale(model, scale);
        return model;
    }
};