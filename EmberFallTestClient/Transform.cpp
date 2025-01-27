#include "pch.h"
#include "Transform.h"

Transform::Transform() { }

Transform::~Transform() { }

Transform::Transform(const Transform& other) {
    ::memcpy(this, &other, sizeof(Transform));
}

Transform::Transform(Transform&& other) noexcept {
    ::memcpy(this, &other, sizeof(Transform));
}

Transform& Transform::operator=(const Transform& other) {
    ::memcpy(this, &other, sizeof(Transform));
    return *this;
}

Transform& Transform::operator=(Transform&& other) noexcept {
    ::memcpy(this, &other, sizeof(Transform));
    return *this;
}

glm::vec3 Transform::GetPosition() const {
    return mPosition;
}

glm::vec3 Transform::GetRotation() const {
    return mRotation;
}

glm::vec3 Transform::GetLook() const {
    return mLook;
}

glm::vec3 Transform::GetScale() const {
    return mScale;
}

glm::mat4 Transform::GetWorld() const {
    return mWorld;
}

void Transform::MoveTo(const glm::vec3& toPos) {
    mPosition = toPos;
}

void Transform::Move(const glm::vec3& moveVec) {
    mPosition += moveVec;
}

void Transform::Rotate(const glm::vec3& angles) {
    mRotation += angles;
}

void Transform::RotateTo(const glm::vec3& toVec) { }

void Transform::Look(const glm::vec3& look) {
    mLook = look;
}

void Transform::Scale(const glm::vec3& scale) {
    mScale = scale;
}

void Transform::Update() {
    mWorld = glm::translate(mPosition) * glm::yawPitchRoll(mRotation.y, mRotation.x, mRotation.z);
}
