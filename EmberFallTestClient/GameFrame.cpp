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
    mNetworkCore = std::make_shared<ClientCore>();
    mNetworkCore->Init();
    if (false == mNetworkCore->Start("127.0.0.1", 7777)) {
        ::exit(EXIT_FAILURE);
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

    mScene->ProcessPackets(mNetworkCore);
    mScene->Update();
    mScene->SendUpdateResult(mNetworkCore);
    mScene->Render();
}

void GameFrame::GameLoop() {
	while (false == glfwWindowShouldClose(mMainWindow->GetWindow())) {
		AdvanceFrame();
        glfwPollEvents();
	}
}
