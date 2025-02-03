#pragma once

class Window;

class Camera {
public:
	Camera() = default;
	Camera(std::shared_ptr<Window> window, const SimpleMath::Vector3& EYE, const SimpleMath::Vector3& AT);

	~Camera() = default;
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;

public:
	virtual void Render(const std::shared_ptr<class Shader>& curShader);
	virtual void Update(float deltaTime, const SimpleMath::Vector3& position=SimpleMath::Vector3::Zero, const SimpleMath::Vector3& look=SimpleMath::Vector3::Zero);

	SimpleMath::Vector3 GetViewPoint() const { return mAt; }
	SimpleMath::Vector3 GetPosition() { return mEye; }

public:
	void UpdateBasisAxis();

protected:
	// To get aspect
	std::shared_ptr<Window> mWindow{};

	glm::mat4 mProjection{ identity };
	glm::mat4 mView{ identity };

protected:
	SimpleMath::Vector3 mEye{};
	SimpleMath::Vector3 mAt{};
	SimpleMath::Vector3 mUp{ 0.f,1.f,0.f };

	float mFovY = 45.f;
	float mNearZ = 0.1f;
	float mFarZ = 1000.f;

	float mAspect{};

	SimpleMath::Vector3 mBasisX{ 1.f,0.f,0.f };
	SimpleMath::Vector3 mBasisY{ 0.f,1.f,0.f };
	SimpleMath::Vector3 mBasisZ{ 0.f,0.f,1.f };
};