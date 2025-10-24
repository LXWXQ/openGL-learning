#include <memory>
#include "Transform.h"
#include "Model.h" // 假设你已经有了Model.h

// Entity.h
class Entity {
public:
    Transform transform;//位移 旋转  缩放
    std::shared_ptr<Model> model; // 使用shared_ptr以便多个实体可以共享同一个模型资源
    std::shared_ptr<Mesh>  mesh;
    Entity(std::shared_ptr<Model> pModel) : model(std::move(pModel)) {}
    Entity(std::shared_ptr<Mesh> pMesh) : mesh(std::move(pMesh)) {}
};