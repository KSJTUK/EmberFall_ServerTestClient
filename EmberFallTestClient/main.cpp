#include "pch.h"
#include "GameObject.h"
#include "Input.h"
#include "Window.h"
#include "Camera.h"

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

	std::shared_ptr<Model> model = std::make_shared<Model>("object/cube.obj");
	model->Init();

	std::shared_ptr<GameObject> obj1 = std::make_shared<GameObject>();
	obj1->ResetModel(model);
	shader->RegisterRenderingObject(obj1);

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

		{
			// Render
			glClearColor(0.5f, 0.5f, 0.5f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			//shader->SetUniformMat4("viewProj", GL_FALSE, identity);
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

	//clientCore->End();
}