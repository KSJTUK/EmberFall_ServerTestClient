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

    auto player = std::make_shared<GameObject>(std::make_shared<Model>("object/cube.obj", "textures/container.jpg"), glm::vec3{0.0f, 0.0f, 0.0f});

    auto cube = std::make_shared<Model>("object/cube.obj");
    pointLight->SetModel(cube);
        
    mPlayers.emplace(player->GetId(), player); // dummy

    auto objOrigin = std::make_shared<GameObject>(std::make_shared<Model>("object/cube.obj"), glm::vec3{0.0f, 0.0f, 0.0f});
    mainShader->RegisterRenderingObject({ objOrigin });
    for (auto& obj : mObjects) {
        obj = objOrigin->Clone();
    }

    mainShader->SetCamera(mCamera);

    mShaders.insert(std::make_pair("mainShader", mainShader));

    RegisterPacketProcessFunctions();
}

GameScene::~GameScene() { }

void GameScene::RegisterPacketProcessFunctions() {
    mPacketProcessor.RegisterProcessFn(PacketType::PACKET_PROTOCOL_VERSION, [=](PacketHeader* header) { ProcessPacketProtocolVersion(header);  });
    mPacketProcessor.RegisterProcessFn(PacketType::PACKET_NOTIFY_ID, [=](PacketHeader* header) { ProcessNotifyId(header); });
    mPacketProcessor.RegisterProcessFn(PacketType::PACKET_PLAYER, [=](PacketHeader* header) { ProcessPlayerPacket(header); });
    mPacketProcessor.RegisterProcessFn(PacketType::PACKET_OBJECT, [=](PacketHeader* header) { ProcessObjectPacket(header); });
    mPacketProcessor.RegisterProcessFn(PacketType::PACKET_OBJECT_APPEARED, [=](PacketHeader* header) { ProcessObjectAppeared(header); });
    mPacketProcessor.RegisterProcessFn(PacketType::PACKET_OBJECT_DISAPPEARED, [=](PacketHeader* header) { ProcessObjectDisappeared(header); });
    mPacketProcessor.RegisterProcessFn(PacketType::PACKET_OBJECT_DEAD, [=](PacketHeader* header) { ProcessObjectDead(header); });
    mPacketProcessor.RegisterProcessFn(PacketType::PACKET_RESTORE_HEALTH, [=](PacketHeader* header) { ProcessRestoreHP(header); });
    mPacketProcessor.RegisterProcessFn(PacketType::PACKET_USE_ITEM, [=](PacketHeader* header) { ProcessUseItem(header); });
    mPacketProcessor.RegisterProcessFn(PacketType::PACKET_PLAYER_EXIT, [=](PacketHeader* header) { ProcessPlayerExit(header); });
    mPacketProcessor.RegisterProcessFn(PacketType::PACKET_ATTACKED, [=](PacketHeader* header) { ProcessObjectAttacked(header); });
    mPacketProcessor.RegisterProcessFn(PacketType::PAKCET_ACQUIRED_ITEM, [=](PacketHeader* header) { ProcessAcquiredItem(header); });
}

void GameScene::ProcessPackets() {
#ifndef STAND_ALONE
    auto packetHandler = gNetworkCore->GetPacketHandler();
    auto& buffer = packetHandler->GetBuffer();

    mPacketProcessor.ProcessPackets(buffer);
#endif
}

void GameScene::Update() {
    mMainTimer->Update();

    const float deltaTime = mMainTimer->GetDeltaTime();
    for (auto& [id, obj] : mPlayers) {
        obj->Update(deltaTime);
    }

    for (auto& obj : mObjects) {
        obj->Update(deltaTime);
    }
}

void GameScene::SendUpdateResult() {
#ifndef STAND_ALONE
    auto myId = gNetworkCore->GetSessionId();

    PacketCS::PacketKeyInput inputPacket{ sizeof(PacketCS::PacketKeyInput), PacketType::PACKET_KEYINPUT, myId };
    auto& keyList = Input::GetStateChangedKeys();
    for (auto key : keyList) {
        inputPacket.key = key.key;
        inputPacket.down = key.state;
        gNetworkCore->Send(&inputPacket);
    }

    PacketCS::PacketCamera game{ sizeof(PacketCS::PacketCamera), PacketType::PACKET_CAMERA, myId };
    game.look = mPlayers[myId]->GetTransform().GetLook();
    gNetworkCore->Send(&game);
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

void GameScene::ProcessNotifyId(PacketHeader* header) {
    std::cout << std::format("INIT SESSION ID : {}\n", header->id);

    gNetworkCore->InitSessionId(header->id);
    auto myId = header->id;

    mPlayers[myId] = mPlayers[INVALID_SESSION_ID];
    mPlayers.erase(INVALID_SESSION_ID);
    mPlayers[myId]->ResetCamera(mCamera);
    mPlayers[myId]->CreateComponent<InputComponent>();
    mPlayers[myId]->SetColor(SimpleMath::Vector3{ RAND_COLOR, RAND_COLOR, RAND_COLOR });
    mShaders["mainShader"]->RegisterRenderingObject(mPlayers[myId]);
}

void GameScene::ProcessPacketProtocolVersion(PacketHeader* header) {
    auto protocolVersion = reinterpret_cast<PacketProtocolVersion*>(header);
    if (PROTOCOL_VERSION_MAJOR != protocolVersion->major or PROTOCOL_VERSION_MINOR != protocolVersion->minor) {
        gNetworkCore->CloseSession();
        //glfwSetWindowShouldClose()

        MessageBox(nullptr, L"ERROR!!!!!\nProtocolVersion Mismatching", L"", MB_OK | MB_ICONERROR);
        ::exit(0);
    }
}

void GameScene::ProcessPlayerPacket(PacketHeader* header) {
    auto playerPacket = reinterpret_cast<PacketSC::PacketPlayer*>(header);
    auto myId = gNetworkCore->GetSessionId();
    auto it = mPlayers.find(header->id);
    if (it == mPlayers.end()) {
        mPlayers[header->id] = mPlayers[myId]->Clone();
    }

    mPlayers[header->id]->SetPosition(playerPacket->position);
    if (myId != header->id) {
        //mPlayers[header->id]->SetRotation(objPacket->rotation);
    }
}

void GameScene::ProcessObjectPacket(PacketHeader* header) {
    auto objPacket = reinterpret_cast<PacketSC::PacketObject*>(header);

    auto object = mObjects[objPacket->objId];
    object->SetPosition(objPacket->position);
}

void GameScene::ProcessObjectDead(PacketHeader* header) {
    auto objPacket = reinterpret_cast<PacketSC::PacketObjectDead*>(header);
    auto object = mObjects[objPacket->objId];
    object->SetActive(false);
}

void GameScene::ProcessObjectAppeared(PacketHeader* header) {
    auto objPacket = reinterpret_cast<PacketSC::PacketObjectAppeared*>(header);
    auto object = mObjects[objPacket->objId];
    object->SetActive(true);
    object->SetPosition(objPacket->position);
}

void GameScene::ProcessObjectDisappeared(PacketHeader* header) {
    auto objPacket = reinterpret_cast<PacketSC::PacketObjectDisappeared*>(header);
    auto object = mObjects[objPacket->objId];
    object->SetActive(false);
}

void GameScene::ProcessPlayerExit(PacketHeader* header) {
    auto exitPacket = reinterpret_cast<PacketSC::PacketObject*>(header);

    if (not mPlayers.contains(header->id)) {
        return;
    }

    mPlayers.erase(header->id);
}

void GameScene::ProcessAcquiredItem(PacketHeader* header) {
    auto acquirePacket = reinterpret_cast<PacketSC::PacketAcquireItem*>(header);
    // TODO...
}

void GameScene::ProcessObjectAttacked(PacketHeader* header) {
    auto attackedPacket = reinterpret_cast<PacketSC::PacketAttacked*>(header);
    // TODO...
}

void GameScene::ProcessUseItem(PacketHeader* header) {
    auto useItemPacket = reinterpret_cast<PacketSC::PacketUseItem*>(header);
    // TODO...
}

void GameScene::ProcessRestoreHP(PacketHeader* header) {
    auto restoreHPPacket = reinterpret_cast<PacketSC::PacketRestoreHP*>(header);
    // TODO...
}
