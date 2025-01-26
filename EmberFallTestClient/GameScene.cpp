#include "pch.h"
#include "GameScene.h"
#include "Input.h"
#include "Window.h"
#include "Camera.h"
#include "Shader.h"
#include "Lighting.h"
#include "Model.h"
#include "GameObject.h"
#include "GameTimer.h"

GameScene::GameScene(std::shared_ptr<Window> mainWindow)
    : mMainWindow{ mainWindow }, mMainTimer{ std::make_unique<GameTimer>() },
    mCamera{ std::make_shared<FreeCamera>(mMainWindow, glm::vec3{ 10.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f } ) } {
    auto mainShader = std::make_shared<Shader>(static_shader);

    auto pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(glm::vec3{ 3.f, 3.f, 3.f });

    auto spotLight = std::make_shared<SpotLight>();
    auto globalLight = std::make_shared<DirectionLight>();
    mainShader->RegisterLights({ pointLight, spotLight, globalLight });

    std::shared_ptr<Model> model = std::make_shared<Model>("object/cube.obj", "textures/container.jpg");
    auto object = std::make_shared<GameObject>(model);
    mObjects.push_back(object);

    mainShader->SetCamera(mCamera);
    mainShader->RegisterLights({ pointLight, spotLight, globalLight });
    mainShader->RegisterRenderingObject(object);

    mShaders.insert(std::make_pair("mainShader", mainShader));
}

GameScene::~GameScene()
{
}

void GameScene::Update() {
    mMainTimer->Update();

    const float deltaTime = mMainTimer->GetDeltaTime();
    mCamera->Update(deltaTime);
    for (auto& obj : mObjects) {
        obj->Update(deltaTime);
    }
}

void GameScene::Render() {
    // Render
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (auto& [tag, shader] : mShaders) {
        shader->Render();
    }

    glfwSwapBuffers(mMainWindow->GetWindow());
}
