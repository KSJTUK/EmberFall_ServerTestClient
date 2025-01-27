#pragma once

class Shader;

constexpr float DIRECTION_MINANGLE = 0.f;
constexpr float DIRECTION_MAXANGLE = 180.f;
constexpr float DAYTIME_SPEED = 0.002f;
constexpr float DAY_LIGHT_MAX_AMBIENT = 0.5f;
constexpr glm::vec3 ROTATE_AXIS_Z = { 0.f, 0.f, 1.f };

enum class LightType : BYTE {
	POINT,
	SPOT,
	DIRECTION
};

class Light abstract {
public:
	Light(LightType type);
	virtual ~Light();

public:
	LightType GetType() const;
	glm::vec3& GetDirLightAmbient() { return mAmbient; }
	glm::vec3& GetDirLightColor() { return mLightColor; }
	glm::vec3 GetPosition();

	void ChangeDirection(const glm::vec3& direction);

	void ChangeAmbient(const glm::vec3& ambient);
	void ChangeLightColor(const glm::vec3& color);
	void SetPosition(const glm::vec3& position);
	void SetLinearQuadrant(float linear, float quadrant);

	virtual void Render(const std::shared_ptr<Shader>&) abstract;

protected:
	glm::vec3 mAmbient{ 0.2f };
	glm::vec3 mLightColor{ 1.f };
	
	glm::vec3 mLightDirection{ };
	glm::vec3 mLightPosition{ };

	float mLinear{ 0.027f };
	float mQuadratic{ 0.0014f };
	const float mConstant{ 1.f };

private:
	LightType mType;
};

class PointLight : public Light {
public:
	PointLight();
	~PointLight();

public:
	void SetModel(std::shared_ptr<class Model> lightModel);

public:
	virtual void Render(const std::shared_ptr<Shader>& curShader) override;
};

class SpotLight : public Light {
public:
	SpotLight();
	~SpotLight();

public:
	void SetOutterCutOff(float cutOff, float outterCutoff);

	virtual void Render(const std::shared_ptr<Shader>& curShader) override;

private:
	float mCutOff{ std::cosf(glm::radians(12.5f)) };
	float mOutterCutOff{ std::cosf(glm::radians(17.5f)) };
};

class DirectionLight : public Light {
public:
	DirectionLight();
	~DirectionLight();

private:
	float mDirectionAngle{ 0.f };
	float mAngleDir{ 1.f };

public:
	void DayUpdate(float deltaTime);

public:
	virtual void Render(const std::shared_ptr<Shader>& curShader) override;
};