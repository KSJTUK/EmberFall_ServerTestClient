#include "pch.h"
#include "GameFrame.h"
#include "Input.h"
#include "Window.h"
#include "GameScene.h"

GameFrame::GameFrame()
    : mMainWindow{ std::make_shared<Window>() }, mScene{ std::make_unique<GameScene>(mMainWindow) } {
    Input::SetMainWindow(mMainWindow);

    // 깊이 테스트 설정
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

#ifndef STAND_ALONE
    mNetworkCore = std::make_shared<Clientcore>();
    mNetworkCore->Init();
    if (false == mNetworkCore->Start("127.0.0.1", 7777)) {
        return EXIT_FAILURE;
    }
#endif
}

GameFrame::~GameFrame() {
#ifndef STAND_ALONE
    mNetworkCore->End();
#endif
}

void GameFrame::ResizeWindow() { }

void GameFrame::AdvanceFrame() {
	Input::Clear();
    Input::Update();		

    mScene->Update();
    mScene->Render();
}

void GameFrame::GameLoop() {
	while (false == glfwWindowShouldClose(mMainWindow->GetWindow())) {
#ifndef STAND_ALONE
		{
			// Networking - Process Recieved Packets
			auto packetHandler = clientCore->GetPacketHandler();
			auto& buffer = packetHandler->GetBuffer();

			PacketHeader header{ };
			while (not buffer.IsReadEnd()) {
				buffer.Read(header);
				switch (header.type) {
				case PacketType::PT_NOTIFYING_ID:
					std::cout << std::format("INIT SESSION ID : {}\n", header.id);
					clientCore->InitSessionId(header.id);
					break;

				default:
					std::cout << "PACKET ERROR" << std::endl;;
					break;
				}
			}
		}
#endif

		AdvanceFrame();

#ifndef STAND_ALONE
		{
			// Networking - Send Input Results
			PacketInput inputPacket{ sizeof(PacketInput), PacketType::PT_INPUT_CS, clientCore->GetSessionId() };
			auto& keyList = Input::GetStateChangedKeys();
			for (auto key : keyList) {
				inputPacket.key = key;
				clientCore->Send(&inputPacket, inputPacket.size);
			}
		}
#endif

		glfwPollEvents();
	}
}
