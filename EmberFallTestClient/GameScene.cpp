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
#include "Terrain.h"

#include "InputComponent.h"
#include "TestComponent.h"

GameScene::GameScene(std::shared_ptr<Window> mainWindow)
    : mMainWindow{ mainWindow }, mMainTimer{ std::make_unique<GameTimer>() },
    mCamera{ std::make_shared<Camera>(mMainWindow, SimpleMath::Vector3{ 0.0f, 1.0f, 0.0f }, SimpleMath::Vector3{ 0.0f, 0.0f, -1.0f }) } {

    mTerrain = std::make_unique<Terrain>(glm::vec2{ 1000.0f, 1000.0f });

    auto mainShader = std::make_shared<Shader>(static_shader);

    auto pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(glm::vec3{ -10.f, 0.f, -10.f });
    pointLight->SetLinearQuadrant(0.0f, 0.0f);

    auto globalLight = std::make_shared<DirectionLight>();
    globalLight->ChangeDirection(glm::vec3{ 0.0f, -1.0f, -1.0f });
    globalLight->SetLinearQuadrant(0.0f, 0.0f);
    mainShader->RegisterLights({ pointLight, globalLight });

    auto player = std::make_shared<GameObject>(std::make_shared<Model>("object/cube.obj"), glm::vec3{ 0.0f, 0.0f, 0.0f });

    auto cube = std::make_shared<Model>("object/cube.obj");
    pointLight->SetModel(cube);
        
    mPlayers.emplace(player->GetId(), player); // dummy

    auto obj = std::make_shared<GameObject>(std::make_shared<Model>("object/cube.obj", "textures/container.jpg"), glm::vec3{0.0f, 0.0f, 0.0f});
    obj->CreateComponent<TestComponent>();
    mObjects.emplace_back(obj);
    mainShader->RegisterRenderingObject({ obj });

    float dist{ 100.0f };
    for (int i = 0; i < 100; ++i) { // 10 * 10 * 10, 10.0f
        SimpleMath::Vector3 distFromOrigin{ dist * (i % 10), 0.0f, dist * (i / 10) };
        distFromOrigin = SimpleMath::Vector3{ -500.0f, 0.0f, -500.0f } + distFromOrigin;
        distFromOrigin.y = mTerrain->GetHeight(distFromOrigin.x, distFromOrigin.z, 0.5f);
        auto newObj = obj->Clone();
        newObj->SetPosition(distFromOrigin);
        obj->SetColor(SimpleMath::Vector3::Zero);
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
        case PacketType::PT_NOTIFYING_ID_SC:
            {
                PacketNotifyId id; 
                buffer.Read(id);
            }
            std::cout << std::format("INIT SESSION ID : {}\n", header.id);
            core->InitSessionId(header.id);
            myId = header.id;

            mPlayers[myId] = mPlayers[255];
            mPlayers[myId]->ResetCamera(mCamera);
            mPlayers[myId]->CreateComponent<InputComponent>();
            mPlayers[myId]->SetColor(SimpleMath::Vector3{ RAND_COLOR, RAND_COLOR, RAND_COLOR });
            mShaders["mainShader"]->RegisterRenderingObject(mPlayers[myId]);
            break;

        case PacketType::PT_GAMEOBJ_SC:
            {
                PacketGameObj objPacket{ };
                buffer.Read(objPacket); 
                auto it = mPlayers.find(header.id);
                if (it == mPlayers.end()) {
                    mPlayers[header.id] = mPlayers[myId]->Clone();
                }

                mPlayers[header.id]->SetPosition(objPacket.position);
                if (myId != header.id) {
                    mPlayers[header.id]->SetRotation(objPacket.rotation);
                }
                mPlayers[header.id]->Scale(objPacket.scale);
                mPlayers[header.id]->SetColor(objPacket.color);
            }
            break;

        default:
            std::cout << "PACKET ERROR Size:" << static_cast<INT32>(header.size) << " PacketType: " << static_cast<INT32>(header.type) << std::endl;
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
    auto myId = core->GetSessionId();

    PacketInput inputPacket{ sizeof(PacketInput), PacketType::PT_INPUT_CS, core->GetSessionId() };
    auto& keyList = Input::GetStateChangedKeys();
    for (auto key : keyList) {
        inputPacket.key = key;
        core->Send(&inputPacket);
    }

    PacketGameObjCS game{ sizeof(PacketGameObjCS), PacketType::PT_GAMEOBJ_CS, core->GetSessionId() };
    game.rotation = mPlayers[myId]->GetTransform().GetRotation();
    core->Send(&game);
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

    mTerrain->Render(mCamera);

    glfwSwapBuffers(mMainWindow->GetWindow());
}
