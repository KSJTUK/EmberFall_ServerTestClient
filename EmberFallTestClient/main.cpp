#include "pch.h"
#include "GameObject.h"
#include "Input.h"
#include "Renderer.h"

#define STAND_ALONE

#include "Shader.h"

int main()
{
	Renderer renderer{ };

	std::shared_ptr<ClientCore> clientCore = std::make_shared<ClientCore>();
	clientCore->Init();

#ifndef STAND_ALONE
	if (false == clientCore->Start("127.0.0.1", 7777)) {
		return EXIT_FAILURE;
	}
	std::cout << "\nThis Program Use This GPU : " << glGetString(GL_RENDERER) << std::endl; 
#endif

	Shader shader{ };
	shader.CreateShaders(static_shader);

	while (false == glfwWindowShouldClose(renderer.GetWindow())) {
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

			glfwSwapBuffers(renderer.GetWindow());
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

	clientCore->End();
}