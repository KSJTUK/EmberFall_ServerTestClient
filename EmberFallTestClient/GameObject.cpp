#include "pch.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"

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
}

void GameObject::Render() {
    if (nullptr == mModel) {
        return;
    }

    BindingTexture();
    mWorldMat *= glm::yawPitchRoll(0.0001f, 0.0001f, 0.0001f);
    mOwnShader->SetUniformMat4("world", GL_FALSE, mWorldMat);

    mModel->Render();
}
