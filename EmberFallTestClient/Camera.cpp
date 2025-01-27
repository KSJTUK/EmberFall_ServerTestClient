#include "pch.h"
#include "Camera.h"
#include "Window.h"
#include "pch.h"
#include "Camera.h"
#include "Shader.h"
#include "Input.h"

constexpr auto CAMERA_SPEED = 20.f;

Camera::Camera(std::shared_ptr<Window> window, glm::vec3 EYE, glm::vec3 AT) : mWindow(window), mEye(EYE), mAt(AT) {
	int width, height;
	glfwGetFramebufferSize(mWindow->GetWindow(), &width, &height);
	mAspect = static_cast<float>(width) / static_cast<float>(height);

	mBasisZ = glm::normalize(-mAt);
	mBasisX = glm::normalize(glm::cross(mUp, mBasisZ));
	mBasisY = glm::cross(mBasisZ, mBasisX);

	mProjection = glm::perspective(mFovY, mAspect, mNearZ, mFarZ);

	float boxHalfWidth{ 6.f / 2.f };
	float boxHeight{ 17.f };
	mBoxSize.first = { glm::vec3{ boxHalfWidth, boxHeight, boxHalfWidth } };
	mBoxSize.second = { glm::vec3{ boxHalfWidth, 0.f, boxHalfWidth} };
	mBoundingBox.first = mEye - mBoxSize.first;
	mBoundingBox.second = mEye + mBoxSize.second;
}

void Camera::Render(const std::shared_ptr<class Shader>& curShader) {
	curShader->SetUniformMat4("viewProj", GL_FALSE, mProjection * mView);
}

void Camera::Update(float deltaTime, glm::vec3 position, glm::vec3 look) {
	int width, height;
	glfwGetFramebufferSize(mWindow->GetWindow(), &width, &height);
	mAspect = static_cast<float>(width) / static_cast<float>(height);

    mEye = position;

	if (false == glm::isCompNull(look, 0.001f).x) {
		mAt = glm::normalize(look);
		mBasisZ = glm::normalize(-mAt);
		mBasisX = glm::normalize(glm::cross(mUp, mBasisZ));
		mBasisY = glm::cross(mBasisZ, mBasisX);
	}

	mView = glm::lookAt(mEye, mEye + mAt, mUp);
}