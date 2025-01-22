#include "pch.h"
#include "GameObject.h"
#include "Input.h"
#include "Renderer.h"

int main()
{
	Renderer renderer{ };

	std::shared_ptr<ClientCore> clientCore = std::make_shared<ClientCore>();
	clientCore->Init();

	if (false == clientCore->Start("127.0.0.1", 7777)) {
		return EXIT_FAILURE;
	}
	std::cout << "\nThis Program Use This GPU : " << glGetString(GL_RENDERER) << std::endl; 

	while (false == glfwWindowShouldClose(renderer.GetWindow())) {
		{
			// Networking - Process Recieved Packets
			auto packetHandler = clientCore->GetPacketHandler();
			auto& buffer = packetHandler->GetBuffer();

			if (0 != buffer.Size()) {
				std::cout << "----------Recv Data----------" << std::endl;
				std::cout << buffer.Data() << std::endl;
			}
		}

		Input::Update();

		{
			// Render
			glClearColor(0.5f, 0.5f, 0.5f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwSwapBuffers(renderer.GetWindow());
		}

		{
			// Networking - Send Input Results
			PacketInput inputPacket{ sizeof(PacketInput), PacketType::PT_INPUT_CS, 0 };
			auto& keyList = Input::GetStateChangedKeys();
			for (auto key : keyList) {
				inputPacket.key = key;
				clientCore->Send(&inputPacket, inputPacket.size);
			}
		}

		Input::Clear();

		glfwPollEvents();
	}

	clientCore->End();
}