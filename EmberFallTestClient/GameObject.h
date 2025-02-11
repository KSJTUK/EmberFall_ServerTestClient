#pragma once

#include "Component.h"
#include "Transform.h"

class GameObject {
public:
    GameObject(std::shared_ptr<class Model> model = nullptr);
    GameObject(std::shared_ptr<class Model> model, glm::vec3 position, glm::vec3 look=glm::vec3{ 0.0f });
    ~GameObject();

public:
    void SetActive(bool active);
    void InitId(SessionIdType id);
    SessionIdType GetId() const;
    Transform& GetTransform();
    float GetSpeed() const;
    SimpleMath::Vector3 GetColor() const;

    void SetComponent(std::shared_ptr<Component> component);
    void SetPosition(const SimpleMath::Vector3& pos);
    void SetRotation(const SimpleMath::Quaternion& rot);
    void Scale(const SimpleMath::Vector3& scale);
    void SetColor(const SimpleMath::Vector3& color);

    template <typename T> requires std::derived_from<T, Component>
    void CreateComponent() {
        auto component = std::make_shared<T>();
        mComponents.push_back(component);
    }

    void UpdateWorld(const SimpleMath::Matrix& world);
    void ResetCamera(std::shared_ptr<class Camera> camera=nullptr);
    void ResetModel(std::shared_ptr<class Model> model=nullptr);
    void ResetShader(std::shared_ptr<class Shader> shader);
    void IsLightObj();
    void BindingTexture();
    void Update(const float deltaTime);

    std::shared_ptr<GameObject> Clone() const;

    void Render();

private:
    SessionIdType mId{ INVALID_SESSION_ID };
    bool mActive{ true };
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