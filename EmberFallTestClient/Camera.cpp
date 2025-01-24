#include "pch.h"
#include "Camera.h"
#include "pch.h"
#include "Camera.h"
#include "Shader.h"
#include "Input.h"

constexpr auto CameraSpeed = 20.f;
constexpr auto MOUSE_SENSITIVE = 0.1f;

Camera::Camera(GLFWwindow* window, glm::vec3 EYE, glm::vec3 AT) : mWindow(window), mEye(EYE), mAt(AT) {
	int width, height;
	glfwGetFramebufferSize(mWindow, &width, &height);
	mAspect = static_cast<float>(width) / static_cast<float>(height);

	mBasisZ = glm::normalize(-mAt);
	mBasisX = glm::normalize(glm::cross(mUp, mBasisZ));
	mBasisY = glm::cross(mBasisZ, mBasisX);

	mProjection = glm::perspective(mFovY, mAspect, mNearZ, mFarZ);
}

FreeCamera::FreeCamera(GLFWwindow* window, glm::vec3 EYE, glm::vec3 AT) :Camera{ window, EYE, AT } {
	float halfWidth{ 6.f / 2.f };
	float height{ 17.f };
	mBoxSize.first = { glm::vec3{ halfWidth, height, halfWidth } };
	mBoxSize.second = { glm::vec3{ halfWidth, 0.f, halfWidth} };
	mBoundingBox.first = mEye - mBoxSize.first;
	mBoundingBox.second = mEye + mBoxSize.second;
}

FreeCamera::~FreeCamera() {
}

void FreeCamera::Render(const std::shared_ptr<Shader>& curShader) {
	curShader->SetUniformMat4("viewProj", GL_FALSE, mProjection * mView);
}

void FreeCamera::Update(float deltaTime) {
	mDeltaTime = deltaTime;
	int width, height;
	glfwGetFramebufferSize(mWindow, &width, &height);
	mAspect = static_cast<float>(width) / static_cast<float>(height);
	mMoveVec = glm::vec3{ 0.f };

	mView = glm::lookAt(mEye, mEye + mAt, mUp);
}
