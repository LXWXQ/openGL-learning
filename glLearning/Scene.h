#include <vector>
#include <memory>
#include "Entity.h"
#include "Camera.h"
#include "Shader.h"

// Scene.h
class Scene {
public:
    Scene() = default;

    // 添加一个实体到场景中
    void addEntity(std::unique_ptr<Entity> entity) {
        m_entities.push_back(std::move(entity));
    }

    // 渲染场景中的所有实体
    void render(ShaderPtr shader, const Camera& camera) {
        // 在实际应用中，你可能需要更复杂的着色器管理
        shader->use();
        shader->setMat4("projection", camera.projection);
        shader->setMat4("view", camera.GetViewMatrix());

        for (const auto& entity : m_entities) {
            if (entity && entity->model) {
                // 设置每个物体的模型矩阵
                shader->setMat4("model", entity->transform.getModelMatrix());
                // 渲染模型
                entity->model->draw(shader);
            }
        }
    }

private:
    std::vector<std::unique_ptr<Entity>> m_entities;
};