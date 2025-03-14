#include "pch.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

GameObject::GameObject(std::shared_ptr<class Model> model)
    : mModel{ model } { }

GameObject::GameObject(std::shared_ptr<class Model> model, glm::vec3 position, glm::vec3 look)
    : mModel{ model } { 
    mTransform.Translate(ConvertVec3(position));
    if (false == glm::isCompNull(look, 0.0001f).x) {
        mTransform.Rotate(ConvertVec3(look));
    }
}

GameObject::~GameObject() { }

void GameObject::SetActive(bool active) {
    mActive = active;
}

void GameObject::InitId(SessionIdType id) {
    mId = id;
}

SessionIdType GameObject::GetId() const {
    return mId;
}

Transform& GameObject::GetTransform() {
    return mTransform;
}

float GameObject::GetSpeed() const {
    return mSpeed;
}

SimpleMath::Vector3 GameObject::GetColor() const {
    return mColor;
}

void GameObject::SetComponent(std::shared_ptr<Component> component) {
    mComponents.push_back(component);
}

void GameObject::SetPosition(const SimpleMath::Vector3& pos) {
    mTransform.SetPosition(pos);
}

void GameObject::SetRotation(const SimpleMath::Quaternion& rot) {
    mTransform.SetRotation(rot);
}

void GameObject::Scale(const SimpleMath::Vector3& scale) {
    mTransform.Scale(scale);
}

void GameObject::SetColor(const SimpleMath::Vector3& color) {
    mColor = color;
}

void GameObject::UpdateWorld(const SimpleMath::Matrix& world) {
    mTransform.SetWorld(world);
}

void GameObject::ResetCamera(std::shared_ptr<class Camera> camera) {
    mCamera = camera;
}

void GameObject::ResetModel(std::shared_ptr<class Model> model) {
    mModel = model;
}

void GameObject::ResetShader(std::shared_ptr<Shader> shader) {
    mOwnShader = shader;
}

void GameObject::IsLightObj() {
    mLightObj = true;
}

void GameObject::BindingTexture() {
    if (mModel->ExistTexture()) {
        mTextured = true;
        mModel->BindingTexture(0);
    }
}

void GameObject::Update(const float deltaTime) { 
    if (not mActive) {
        return;
    }

    for (auto& component : mComponents) {
        component->Update(deltaTime, *this);
    }

    mTransform.Update();
    if (nullptr != mCamera) {
        mCamera->Update(deltaTime,
            mTransform.GetPosition(),
            mTransform.GetLook(),
            SimpleMath::Vector3::Transform((SimpleMath::Vector3{ 0.0f, 1.5f, 6.0f } * mTransform.GetScale()), mTransform.GetRotation())
        ); // temp
    }
}

void GameObject::LateUpdate(const float deltaTime) { }

std::shared_ptr<GameObject> GameObject::Clone() const {
    auto clone = std::make_shared<GameObject>();
    clone->mModel = mModel;
    clone->mOwnShader = mOwnShader;
    clone->mSpeed = mSpeed;
    clone->mColor = mColor;
    clone->mLightObj = mLightObj;
    //for (auto& component : mComponents) {
    //    clone->SetComponent(component);
    //}

    mOwnShader->RegisterRenderingObject(clone);

    return clone;
}

void GameObject::Render() {
    if (nullptr == mModel or false == mActive) {
        return;
    }

    glm::mat4 glmWorld{ };
    auto world = mTransform.GetWorld();
    std::memcpy(&glmWorld, &world, sizeof(glm::mat4));

    BindingTexture();
    mOwnShader->SetUniformInt("textured", static_cast<int>(mTextured));
    mOwnShader->SetUniformInt("lightObj", static_cast<int>(mLightObj));
    if (false == mTextured) {
        mOwnShader->SetUniformVec3("objColor", ConvertVec3(mColor));
    }
    mOwnShader->SetUniformMat4("world", GL_FALSE, glmWorld);

    mModel->Render();
}
