#pragma once

#include "Component.h"
#include "Transform.h"

class GameObject {
public:
    GameObject(std::shared_ptr<class Model> model = nullptr);
    GameObject(std::shared_ptr<class Model> model, glm::vec3 position, glm::vec3 look=glm::vec3{ 0.0f });
    ~GameObject();

public:
    void InitId(SessionIdType id);
    SessionIdType GetId() const;
    Transform& GetTransform();
    float GetSpeed() const;

    void SetComponent(std::shared_ptr<Component> component);
    void SetPosition(const SimpleMath::Vector3& pos);
    void SetColor(const SimpleMath::Vector3& color);

    template <typename T> requires std::derived_from<T, Component>
    void CreateComponent() {
        auto component = std::make_shared<T>();
        mComponents.push_back(component);
    }

    void ResetCamera(std::shared_ptr<class Camera> camera=nullptr);
    void ResetModel(std::shared_ptr<class Model> model=nullptr);
    void ResetShader(std::shared_ptr<class Shader> shader);
    void IsLightObj();
    void BindingTexture();
    void Update(const float deltaTime);

    std::shared_ptr<GameObject> Clone() const;

    void Render();

private:
    SessionIdType mId{ INVALID_CLIENT_ID };
    bool mTextured{ false };
    bool mLightObj{ false };
    SimpleMath::Vector3 mColor{ 1.0f, 1.0f, 1.0f };

    std::shared_ptr<class Camera> mCamera{ };
    std::shared_ptr<class Shader> mOwnShader{ };
    std::shared_ptr<class Model> mModel{ };
    std::vector<std::shared_ptr<Component>> mComponents{ };

    float mSpeed{ 10.0f };
    Transform mTransform{ };
};