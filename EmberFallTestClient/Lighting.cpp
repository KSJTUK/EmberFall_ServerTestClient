#include "pch.h"
#include "Lighting.h"
#include "Shader.h"

Light::Light(LightType type)
	: mType{ type } { }

Light::~Light() { }

LightType Light::GetType() const {
	return LightType();
}

glm::vec3 Light::GetPosition() {
	return mLightPosition;
}

void Light::ChangeDirection(const glm::vec3& direction) {
	mLightDirection = direction;
}

void Light::ChangeAmbient(const glm::vec3& ambient) {
	mAmbient = ambient;
}

void Light::ChangeLightColor(const glm::vec3& color) {
	mLightColor = color;
}

void Light::SetPosition(const glm::vec3& position) {
	mLightPosition = position;
}

void Light::SetLinearQuadrant(float linear, float quadrant) {
	mLinear = linear;
	mQuadratic = quadrant;
}


PointLight::PointLight() : Light{ LightType::POINT } { }

PointLight::~PointLight() { }

void PointLight::SetModel(std::shared_ptr<class Model> lightModel) {
}

void PointLight::Render(const std::shared_ptr<Shader>& curShader) {
	curShader->SetUniformVec3("pointLight.position", mLightPosition);
			
	curShader->SetUniformVec3("pointLight.ambient", mAmbient);
	curShader->SetUniformVec3("pointLight.diffuse", mLightColor);
	curShader->SetUniformVec3("pointLight.specular", mLightColor);
			
	curShader->SetUniformFloat("pointLight.constant", mConstant);
	curShader->SetUniformFloat("pointLight.linear", mLinear);
	curShader->SetUniformFloat("pointLight.quadratic", mQuadratic);
}

SpotLight::SpotLight() : Light{ LightType::SPOT } { }

SpotLight::~SpotLight() { }


void SpotLight::SetOutterCutOff(float cutOff, float outterCutoff) {
	mCutOff = std::cosf(glm::radians(cutOff));
	mOutterCutOff = std::cosf(glm::radians(outterCutoff));
}

void SpotLight::Render(const std::shared_ptr<Shader>& curShader) {
	curShader->SetUniformVec3("spotLight.position", mLightPosition);
	curShader->SetUniformVec3("spotLight.direction", mLightDirection);

	curShader->SetUniformVec3("spotLight.ambient", mAmbient);
	curShader->SetUniformVec3("spotLight.diffuse", mLightColor);
	curShader->SetUniformVec3("spotLight.specular", mLightColor);

	curShader->SetUniformFloat("spotLight.cutOff", mCutOff);
	curShader->SetUniformFloat("spotLight.outterCutOff", mOutterCutOff);

	curShader->SetUniformFloat("spotLight.constant", mConstant);
	curShader->SetUniformFloat("spotLight.linear", mLinear);
	curShader->SetUniformFloat("spotLight.quadratic", mQuadratic);
}

DirectionLight::DirectionLight() : Light{ LightType::DIRECTION } { }

DirectionLight::~DirectionLight() { }

void DirectionLight::DayUpdate(float deltaTime) {
	mDirectionAngle += DAYTIME_SPEED * mAngleDir * deltaTime;
	if (mDirectionAngle > DIRECTION_MAXANGLE) {
		mDirectionAngle = DIRECTION_MAXANGLE;
		mAngleDir = -1.f;
	}
	else if (mDirectionAngle < DIRECTION_MINANGLE) {
		mDirectionAngle = DIRECTION_MINANGLE;
		mAngleDir = 1.f;
	}
	mLightDirection = glm::rotate(mLightDirection, glm::radians(mDirectionAngle), ROTATE_AXIS_Z);
	mLightColor = glm::vec3{ std::sinf(mDirectionAngle) };
	mAmbient = glm::vec3{ glm::clamp(std::sinf(mDirectionAngle) - (1.f - DAY_LIGHT_MAX_AMBIENT), 0.f, DAY_LIGHT_MAX_AMBIENT) };
}

void DirectionLight::Render(const std::shared_ptr<Shader>& curShader) {
	curShader->SetUniformVec3("dirLight.direction", mLightDirection);
	
	curShader->SetUniformVec3("dirLight.ambient", mAmbient);
	curShader->SetUniformVec3("dirLight.diffuse", mLightColor);
	curShader->SetUniformVec3("dirLight.specular", mLightColor);
}
