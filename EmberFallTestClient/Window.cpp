#include "pch.h"
#include "Window.h"

Window::Window() {
	// glfw 라이브러리 초기화
	if (false == glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// OpenGL 버전 설정
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 윈도우 정보 초기 설정
	INT32 width = 800;
	INT32 height = 600;
	INT32 x = 0;
	INT32 y = 0;
	std::string windowTitle = "EmberFallTestClient";
	mWindow = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	if (nullptr == mWindow) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// 생성한 윈도우를 현재 메인 윈도우로 설정
	glfwMakeContextCurrent(mWindow);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		exit(EXIT_FAILURE);
	}

	// 뷰포트 설정
	glViewport(0, 0, width, height);
	glfwSwapInterval(0);
}

Window::~Window() { }

GLFWwindow* Window::GetWindow() {
	return mWindow;
}

void Window::ResizeWindow(INT32 width, INT32 height) {
}
