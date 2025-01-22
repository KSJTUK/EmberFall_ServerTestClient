#include "pch.h"
#include "GameObject.h"
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


	std::chrono::milliseconds delay{ 1000 };
	while (false == glfwWindowShouldClose(renderer.GetWindow())) {
		std::this_thread::sleep_for(delay);

		{
			// Networking
			auto packetHandler = clientCore->GetPacketHandler();
			auto& buffer = packetHandler->GetBuffer();

			if (0 != buffer.Size()) {
				std::cout << "----------Recv Data----------" << std::endl;
				std::cout << buffer.Data() << std::endl;
			}
		}

		{
			// Render
			glClearColor(0.5f, 0.5f, 0.5f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwSwapBuffers(renderer.GetWindow());
		}

		glfwPollEvents();
	}

	clientCore->End();
}