#include "pch.h"
#include "Camera.h"
#include "Window.h"
#include "pch.h"
#include "Camera.h"
#include "Shader.h"
#include "Input.h"

constexpr auto CameraSpeed = 20.f;
constexpr auto MOUSE_SENSITIVE = 0.1f;

Camera::Camera(std::shared_ptr<Window> window, glm::vec3 EYE, glm::vec3 AT) : mWindow(window), mEye(EYE), mAt(AT) {
	int width, height;
	glfwGetFramebufferSize(mWindow->GetWindow(), &width, &height);
	mAspect = static_cast<float>(width) / static_cast<float>(height);

	mBasisZ = glm::normalize(-mAt);
	mBasisX = glm::normalize(glm::cross(mUp, mBasisZ));
	mBasisY = glm::cross(mBasisZ, mBasisX);

	mProjection = glm::perspective(mFovY, mAspect, mNearZ, mFarZ);
}

FreeCamera::FreeCamera(std::shared_ptr<Window> window, glm::vec3 EYE, glm::vec3 AT) : Camera{ window, EYE, AT } {
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
	glfwGetFramebufferSize(mWindow->GetWindow(), &width, &height);
	mAspect = static_cast<float>(width) / static_cast<float>(height);
	mMoveVec = glm::vec3{ 0.f };

    if (Input::GetState(GLFW_KEY_W)) {
        mMoveVec -= mBasisZ;
        mEye -= mBasisZ * deltaTime * CameraSpeed;
    }

    if (Input::GetState(GLFW_KEY_S)) {
        mMoveVec += mBasisZ;
        mEye += mBasisZ * deltaTime * CameraSpeed;
    }


    if (Input::GetState(GLFW_KEY_A)) {
        mMoveVec -= mBasisX;
        mEye -= mBasisX * deltaTime * CameraSpeed;
    }


    if (Input::GetState(GLFW_KEY_D)) {
        mMoveVec += mBasisX;
        mEye += mBasisX * deltaTime * CameraSpeed;
    }

    if (Input::GetState(GLFW_KEY_Q)) {
        mMoveVec -= mBasisY;
        mEye -= mBasisY * deltaTime * CameraSpeed;
    }


    if (Input::GetState(GLFW_KEY_E)) {
        mMoveVec += mBasisY;
        mEye += mBasisY * deltaTime * CameraSpeed;
    }

	//m_cancelMove = false;

	glm::vec2 deltaMouse = Input::GetDeltaMouse();

	if (!(deltaMouse.x == 0 and deltaMouse.y == 0)) {
		glm::mat4 cameraRotate{ 1.f };

		cameraRotate = glm::rotate(cameraRotate, glm::radians(-deltaMouse.y * MOUSE_SENSITIVE), mBasisX);
		cameraRotate = glm::rotate(cameraRotate, glm::radians(-deltaMouse.x * MOUSE_SENSITIVE), mBasisY);

		mRotate = cameraRotate;
		mDeltaRotate.y = glm::radians(-deltaMouse.x * MOUSE_SENSITIVE);
		mDeltaRotate.x = glm::radians(-deltaMouse.y * MOUSE_SENSITIVE);

		mAt = glm::normalize(glm::vec3(cameraRotate * glm::vec4(mAt, 1.f)));
		mBasisZ = glm::normalize(-mAt);
		mBasisX = glm::normalize(glm::cross(mUp, mBasisZ));
		mBasisY = glm::cross(mBasisZ, mBasisX);
	}
	else {
		mDeltaRotate = glm::vec3{ };
	}

	mView = glm::lookAt(mEye, mEye + mAt, mUp);
}
