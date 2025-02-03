#include "pch.h"
#include "Camera.h"
#include "Window.h"
#include "pch.h"
#include "Camera.h"
#include "Shader.h"
#include "Input.h"

constexpr auto CAMERA_SPEED = 20.f;

Camera::Camera(std::shared_ptr<Window> window, const SimpleMath::Vector3& EYE, const SimpleMath::Vector3& AT) 
	: mWindow{ window }, mEye{ EYE }, mAt{ AT } {
	int width, height;
	glfwGetFramebufferSize(mWindow->GetWindow(), &width, &height);
	mAspect = static_cast<float>(width) / static_cast<float>(height);

	mAt.Normalize();
	mBasisZ = mAt;

	mBasisX = mUp.Cross(mBasisZ);
	mBasisX.Normalize();

	mBasisY = mBasisZ.Cross(mBasisX);
	mBasisY.Normalize();

	mProjection = glm::perspective(mFovY, mAspect, mNearZ, mFarZ);
}

void Camera::Render(const std::shared_ptr<class Shader>& curShader) {
	curShader->SetUniformMat4("viewProj", GL_FALSE, mProjection * mView);
}

void Camera::Update(float deltaTime, const SimpleMath::Vector3& position, const SimpleMath::Vector3& look) {
	int width, height;
	glfwGetFramebufferSize(mWindow->GetWindow(), &width, &height);
	mAspect = static_cast<float>(width) / static_cast<float>(height);

    mEye = position;

	if (false == IsVector3Zero(look)) {
		mAt = look;
		UpdateBasisAxis();
	}

	mView = ConvertDXMatToGLMat(DirectX::XMMatrixLookAtRH(mEye, mEye + mAt, mUp));
}

void Camera::UpdateBasisAxis() {
	mAt.Normalize();
	mBasisZ = mAt;

	mBasisX = mUp.Cross(mBasisZ);
	mBasisX.Normalize();

	mBasisY = mBasisZ.Cross(mBasisX);
	mBasisY.Normalize();
}
