#pragma once

class Window;

class Camera abstract {
public:
	Camera() = default;
	Camera(std::shared_ptr<Window> window, glm::vec3 EYE, glm::vec3 AT);

	~Camera() = default;
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;

public:
	virtual void Render(const std::shared_ptr<class Shader>& curShader) = 0;
	virtual void Update(float deltaTime) = 0;

	const glm::mat4* GetViewPtr() { return &mView; };
	const glm::mat4* GetProjectionPtr() { return &mProjection; };
	const glm::mat4* GetRotateMatPtr() { return &mRotate; };
	const glm::vec3* GetPositionPtr() { return &mEye; };
	const glm::vec3* GetBasisZPtr() { return &mBasisZ; };
	const glm::vec3* GetBasisYPtr() { return &mBasisY; };
	const glm::vec3* GetBasisXPtr() { return &mBasisX; };
	glm::vec3 GetDeltaAngle() { return mDeltaRotate; }
	glm::vec3 GetViewPoint() const { return mAt; }
	glm::vec3& GetPosition() { return mEye; }

protected:
	// To get aspect
	std::shared_ptr<Window> mWindow{};

	glm::mat4 mProjection{ identity };
	glm::mat4 mView{ identity };

protected:
	glm::vec3 mEye{};
	glm::vec3 mAt{};
	glm::vec3 mUp{ 0.f,1.f,0.f };

	float mFovY = 45.f;
	float mNearZ = 0.1f;
	float mFarZ = 1000.f;

	float mAspect{};

	glm::vec3 mBasisX{ 1.f,0.f,0.f };
	glm::vec3 mBasisY{ 0.f,1.f,0.f };
	glm::vec3 mBasisZ{ 0.f,0.f,1.f };

	glm::mat4 mRotate{ 0.f };
	glm::vec3 mDeltaRotate{ 0.f };
};

class FreeCamera : public Camera {
public:
	FreeCamera(std::shared_ptr<Window> window, glm::vec3 EYE, glm::vec3 AT);
	~FreeCamera();

public:
	virtual void Render(const std::shared_ptr<class Shader>& curShader) override;
	virtual void Update(float deltaTime) override;

private:
	float mDeltaTime{ };
	std::pair<glm::vec3, glm::vec3> mBoxSize{ };
	std::pair<glm::vec3, glm::vec3> mBoundingBox{ };
	glm::vec3 mMoveVec{ };
	bool mCancelMove{ false };
};