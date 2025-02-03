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

#include "InputComponent.h"
#include "TestComponent.h"

GameScene::GameScene(std::shared_ptr<Window> mainWindow)
    : mMainWindow{ mainWindow }, mMainTimer{ std::make_unique<GameTimer>() },
    mCamera{ std::make_shared<Camera>(mMainWindow, SimpleMath::Vector3{ 0.0f, 0.0f, -5.0f }, SimpleMath::Vector3{ 0.0f, 0.0f, -1.0f }) } {
    auto mainShader = std::make_shared<Shader>(static_shader);

    auto pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(glm::vec3{ -3.f, 3.f, -3.f });
    pointLight->SetLinearQuadrant(0.0f, 0.0f);

    auto globalLight = std::make_shared<DirectionLight>();
    globalLight->ChangeDirection(glm::vec3{ 0.0f, -1.0f, -1.0f });
    globalLight->SetLinearQuadrant(0.0f, 0.0f);
    mainShader->RegisterLights({ pointLight, globalLight });

    auto player = std::make_shared<GameObject>(std::make_shared<Model>("object/cube.obj", "textures/container.jpg"), glm::vec3{ 0.0f, 0.0f, 5.0f });
    player->CreateComponent<InputComponent>();
    player->ResetCamera(mCamera);

    auto object2 = std::make_shared<GameObject>(std::make_shared<Model>("object/cube.obj", "textures/container.jpg"), glm::vec3{ 0.0f, 0.0f, 0.0f });
    object2->CreateComponent<TestComponent>();

    auto cube = std::make_shared<Model>("object/cube.obj");
    pointLight->SetModel(cube);
        
    mPlayers.emplace(player->GetId(), player);
    mObjects.emplace_back(object2);
    mainShader->RegisterRenderingObject({ player, object2 });

    float dist{ 10.0f };
    for (int i = 0; i < 1000; ++i) { // 10 * 10 * 10, 10.0f
        SimpleMath::Vector3 distFromOrigin{ dist * (i % 10), dist * (i / 10 % 10), dist * (i / 100) };
        auto newObj = object2->Clone();
        newObj->SetPosition(distFromOrigin);
        distFromOrigin.Normalize();
        object2->SetColor(distFromOrigin);
        mObjects.push_back(newObj);
    }

    mainShader->SetCamera(mCamera);

    mShaders.insert(std::make_pair("mainShader", mainShader));
}

GameScene::~GameScene() { }

void GameScene::ProcessPackets(const std::shared_ptr<ClientCore>& core) {
#ifndef STAND_ALONE
    auto myId = core->GetSessionId();
    auto packetHandler = core->GetPacketHandler();
    auto& buffer = packetHandler->GetBuffer();

    PacketHeader header{ };
    while (not buffer.IsReadEnd()) {
        buffer.Read(header);
        switch (header.type) {
        case PacketType::PT_NOTIFYING_ID:
            std::cout << std::format("INIT SESSION ID : {}\n", header.id);
            core->InitSessionId(header.id);
            myId = header.id;
            break;

        case PacketType::PT_GAMEOBJ_SC:
            std::cout << std::format("OBJECT Info: {}\n", header.id);
            {
                auto it = mPlayers.find(header.id);
                if (mPlayers.end() == it) {
                    mPlayers[header.id] = mPlayers[myId]->Clone();
                }

                //mPlayers[header.id] = 
            }
            break;

        default:
            std::cout << "PACKET ERROR" << std::endl;
            break;
        }
    }
#endif
}

void GameScene::Update() {
    mMainTimer->Update();

    const float deltaTime = mMainTimer->GetDeltaTime();
    mCamera->Update(deltaTime);
    for (auto& [id, obj] : mPlayers) {
        obj->Update(deltaTime);
    }

    for (auto& obj : mObjects) {
        obj->Update(deltaTime);
    }
}

void GameScene::SendUpdateResult(const std::shared_ptr<ClientCore>& core) {
#ifndef STAND_ALONE
    // Networking - Send Input Results
    PacketInput inputPacket{ sizeof(PacketInput), PacketType::PT_INPUT_CS, core->GetSessionId() };
    auto& keyList = Input::GetStateChangedKeys();
    for (auto key : keyList) {
        inputPacket.key = key;
        core->Send(&inputPacket, inputPacket.size);
    }
#endif
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
