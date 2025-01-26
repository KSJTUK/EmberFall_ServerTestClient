#include "pch.h"
#include "GameObject.h"
#include "Input.h"
#include "Window.h"
#include "Camera.h"
#include "Lighting.h"

#define STAND_ALONE

#include "Shader.h"
#include "Model.h"

int main()
{
	std::shared_ptr<Window> window = std::make_shared<Window>();
	Input::SetMainWindow(window);

#ifndef STAND_ALONE
	std::shared_ptr<ClientCore> clientCore = std::make_shared<ClientCore>();
	clientCore->Init();

	if (false == clientCore->Start("127.0.0.1", 7777)) {
		return EXIT_FAILURE;
	}
	std::cout << "\nThis Program Use This GPU : " << glGetString(GL_RENDERER) << std::endl; 
#endif

	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	shader->CreateShaders(static_shader);

	std::shared_ptr<Model> model = std::make_shared<Model>("object/cube.obj", "textures/container.jpg");
	model->Init();

	std::shared_ptr<GameObject> obj1 = std::make_shared<GameObject>();
	obj1->ResetModel(model);
	shader->RegisterRenderingObject(obj1);
	
	std::shared_ptr<FreeCamera> camera = std::make_shared<FreeCamera>(window, glm::vec3{ 0.f, 0.f, 0.f }, glm::vec3{ -1.0f, 0.f, 0.f });
	shader->SetCamera(camera);

	auto pointLight = std::make_shared<PointLight>();
	pointLight->SetPosition(glm::vec3{ 5.f, 5.f, 5.f });

	auto spotLight = std::make_shared<SpotLight>();
	auto globalLight = std::make_shared<DirectionLight>();
	shader->RegisterLight(pointLight);
	//shader->RegisterLight(spotLight);
	//shader->RegisterLight(globalLight);

	// 깊이 테스트 설정
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	while (false == glfwWindowShouldClose(window->GetWindow())) {
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

		Input::Update();
		camera->Update(0.01f);

		{
			// Render
			glClearColor(0.5f, 0.5f, 0.5f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			shader->Render(); 

			glfwSwapBuffers(window->GetWindow());
		}

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

		Input::Clear();

		glfwPollEvents();
	}

#ifndef STAND_ALONE
	clientCore->End();
#endif
}