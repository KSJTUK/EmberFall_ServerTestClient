//#include <GL/glew.h>
//#include <GL/GLFW/glfw3.h>
////#include <GL/GLM/glm.hpp>
////#include <GL/GLM/ext.hpp>
////#include <GL/GLM/gtc/matrix_transform.hpp>
////#include <GL/GLM/gtx/vector_angle.hpp>
////#include <GL/GLM/gtx/euler_angles.hpp>
//
//#include "include/pch.h"
//#include "include/NetworkCore.h"
//#pragma comment(lib, "lib/debug/ServerLib.lib")
//
//int main()
//{
//	// glfw ���̺귯�� �ʱ�ȭ
//	if (false == glfwInit()) {
//		exit(EXIT_FAILURE);
//	}
//
//	// OpenGL ���� ����
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	// ������ ���� �ʱ� ����
//	INT32 width = 1920;
//	INT32 height = 1080;
//	INT32 x = 0;
//	INT32 y = 0;
//	std::string windowTitle = "EmberFallTestClient";
//	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
//
//	if (nullptr == window) {
//		glfwTerminate();
//		exit(EXIT_FAILURE);
//	}
//
//	// ������ �����츦 ���� ���� ������� ����
//	glfwMakeContextCurrent(window);
//
//	glewExperimental = GL_TRUE;
//	if (GLEW_OK != glewInit()) {
//		exit(EXIT_FAILURE);
//	}
//
//	// ����Ʈ ����
//	glViewport(0, 0, width, height);
//	glfwSwapInterval(0);
//
//	std::shared_ptr<ClientCore> clientCore = std::make_shared<ClientCore>();
//	clientCore->Init();
//
//	if (false == clientCore->Start("127.0.0.1", 7777)) {
//		return EXIT_FAILURE;
//	}
//	std::cout << "\nThis Program Use This GPU : " << glGetString(GL_RENDERER) << std::endl;
//
//	std::string str{ "PING!!!" };
//	std::chrono::milliseconds delay{ 100 };
//	while (false == glfwWindowShouldClose(window)) {
//		std::this_thread::sleep_for(delay);
//
//		// Networking
//		auto packetHandler = clientCore->GetPacketHandler();
//		auto& buffer = packetHandler->GetBuffer();
//
//		if (0 != buffer.Size()) {
//			std::cout << "----------Recv Data----------" << std::endl;
//			std::cout << buffer.Data() << std::endl;
//		}
//
//		glClearColor(1.0f, 0.5f, 0.5f, 1.f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		glfwSwapBuffers(window);
//
//		clientCore->Send(str.data(), str.size());
//
//		glfwPollEvents();
//	}
//
//	clientCore->End();
//}