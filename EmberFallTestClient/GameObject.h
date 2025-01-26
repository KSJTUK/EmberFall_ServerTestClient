#pragma once

#include "InputComponent.h"

class GameObject {
public:
    GameObject(std::shared_ptr<class Model> model = nullptr);
    ~GameObject();

public:
    void ResetModel(std::shared_ptr<class Model> model=nullptr);
    void ResetShader(std::shared_ptr<class Shader> shader);
    void BindingTexture();
    void Update(const float deltaTime);
    void Render();

private:
    std::shared_ptr<class Shader> mOwnShader{ };
    std::shared_ptr<class Model> mModel{ };
    glm::mat4 mWorldMat{ identity };
};