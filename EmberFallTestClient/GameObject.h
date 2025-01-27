#pragma once

#include "InputComponent.h"
#include "Transform.h"

class GameObject {
public:
    GameObject(std::shared_ptr<class Model> model = nullptr);
    GameObject(std::shared_ptr<class Model> model, glm::vec3 position, glm::vec3 look=glm::vec3{ 0.0f });
    ~GameObject();

public:
    Transform& GetTransform();
    float GetSpeed() const;

    void SetComponent(std::shared_ptr<Component> component);
    template <typename T> requires std::derived_from<T, Component>
    void CreateComponent() {
        auto component = std::make_shared<T>();
        mComponents.push_back(component);
    }

    void ResetCamera(std::shared_ptr<class Camera> camera=nullptr);
    void ResetModel(std::shared_ptr<class Model> model=nullptr);
    void ResetShader(std::shared_ptr<class Shader> shader);
    void BindingTexture();
    void Update(const float deltaTime);
    void Render();

private:
    std::shared_ptr<class Camera> mCamera{ };
    std::shared_ptr<class Shader> mOwnShader{ };
    std::shared_ptr<class Model> mModel{ };
    std::vector<std::shared_ptr<Component>> mComponents{ };
    float mSpeed{ 10.0f };
    Transform mTransform{ };
};