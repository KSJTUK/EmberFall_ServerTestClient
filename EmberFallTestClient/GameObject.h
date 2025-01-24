#pragma once

#include "InputComponent.h"

class GameObject {
public:
    GameObject() { }  
    ~GameObject() { }

public:
    void ResetModel(std::shared_ptr<class Model> model);
    void ResetShader(std::shared_ptr<class Shader> shader);
    void Update(const float deltaTime);
    void Render();

private:
    std::shared_ptr<class Shader> mOwnShader{ };
    std::shared_ptr<class Model> mModel{ };
    glm::mat4 mWorldMat{ identity };
};