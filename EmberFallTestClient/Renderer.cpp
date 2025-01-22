#include "pch.h"
#include "Renderer.h"

Renderer::Renderer() {
	// glfw ���̺귯�� �ʱ�ȭ
	if (false == glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// OpenGL ���� ����
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ������ ���� �ʱ� ����
	INT32 width = 1080;
	INT32 height = 720;
	INT32 x = 0;
	INT32 y = 0;
	std::string windowTitle = "EmberFallTestClient";
	mWindow = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	if (nullptr == mWindow) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// ������ �����츦 ���� ���� ������� ����
	glfwMakeContextCurrent(mWindow);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		exit(EXIT_FAILURE);
	}

	// ����Ʈ ����
	glViewport(0, 0, width, height);
	glfwSwapInterval(0);
}

Renderer::~Renderer() { }

GLFWwindow* Renderer::GetWindow() {
	return mWindow;
}
