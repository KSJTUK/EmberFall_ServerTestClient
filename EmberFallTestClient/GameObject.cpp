#include "pch.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

GameObject::GameObject(std::shared_ptr<class Model> model)
    : mModel{ model } { }

GameObject::GameObject(std::shared_ptr<class Model> model, glm::vec3 position, glm::vec3 look)
    : mModel{ model } { 
    mTransform.Move(position);
    mTransform.RotateTo(look);
    if (false == glm::isCompNull(look, 0.0001f).x) {
        mTransform.Look(look);
    }
}

GameObject::~GameObject() { }

Transform& GameObject::GetTransform() {
    return mTransform;
}

float GameObject::GetSpeed() const {
    return mSpeed;
}

void GameObject::SetComponent(std::shared_ptr<Component> component) {
    mComponents.push_back(component);
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

void GameObject::BindingTexture() {
    if (mModel->ExistTexture()) {
        mModel->BindingTexture(0);
    }
}

void GameObject::Update(const float deltaTime) { 
    for (auto& component : mComponents) {
        component->Update(deltaTime, *this);
    }

    mTransform.Update();
    if (nullptr != mCamera) {
        mCamera->Update(deltaTime, mTransform.GetPosition(), mTransform.GetLook());
    }
}

void GameObject::Render() {
    if (nullptr == mModel) {
        return;
    }

    BindingTexture();
    mOwnShader->SetUniformMat4("world", GL_FALSE, mTransform.GetWorld());

    mModel->Render();
}
