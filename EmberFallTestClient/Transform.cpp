#include "pch.h"
#include "Transform.h"

Transform::Transform() { }

Transform::~Transform() { }

Transform::Transform(const Transform& other)
    : mPosition{ other.mPosition }, mRotation{ other.mRotation }, mScale{ other.mScale } { }

Transform::Transform(Transform&& other) noexcept
    : mPosition{ other.mPosition }, mRotation{ other.mRotation }, mScale{ other.mScale } { }

Transform& Transform::operator=(const Transform& other) {
    mPosition = other.mPosition;
    mRotation = other.mRotation;
    mScale = other.mScale;

    return *this;
}

Transform& Transform::operator=(Transform&& other) noexcept {
    mPosition = other.mPosition;
    mRotation = other.mRotation;
    mScale = other.mScale;

    return *this;
}

SimpleMath::Vector3 Transform::GetRight() const {
    return DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Right, mRotation);
}

SimpleMath::Vector3 Transform::GetLook() const {
    return DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, mRotation);
}

SimpleMath::Vector3 Transform::GetUp() const {
    return DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Up, mRotation);
}

SimpleMath::Vector3 Transform::GetPosition() const {
    return mPosition;
}

SimpleMath::Quaternion Transform::GetRotation() const {
    return mRotation;
}

SimpleMath::Vector3 Transform::GetScale() const {
    return mScale;
}

SimpleMath::Matrix Transform::GetWorld() const {
    return mWorld;
}

void Transform::SetPosition(const SimpleMath::Vector3& pos) {
    mPosition = pos;
}

void Transform::SetRotation(const SimpleMath::Quaternion& quat) {
    mRotation = quat;
}

void Transform::SetWorld(const SimpleMath::Matrix& world) {
    mWorld = world;
}

void Transform::Translate(const SimpleMath::Vector3& v) {
    mPosition += v;
}

void Transform::Move(const SimpleMath::Vector3& moveVec) {
    Translate(SimpleMath::Vector3::Transform(moveVec, mRotation));
}

void Transform::Rotate(const float yaw, const float pitch, const float roll) {
    auto rot = SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
    mRotation = SimpleMath::Quaternion::Concatenate(rot, mRotation);
    mRotation.Normalize();
}

void Transform::Rotate(const SimpleMath::Vector3& v) {
    auto rot = SimpleMath::Quaternion::CreateFromYawPitchRoll(v);
    mRotation = SimpleMath::Quaternion::Concatenate(rot, mRotation);
    mRotation.Normalize();
}

void Transform::Rotate(const SimpleMath::Quaternion& quat) {
    mRotation = SimpleMath::Quaternion::Concatenate(quat, mRotation);
    mRotation.Normalize();
}

void Transform::RotateSmoothly(const SimpleMath::Quaternion& quat) {

}

void Transform::Scale(const SimpleMath::Vector3& v) {
    mScale = v;
}

void Transform::Update() {
    mWorld = SimpleMath::Matrix::CreateScale(mScale)
        * SimpleMath::Matrix::CreateFromQuaternion(mRotation)
        * SimpleMath::Matrix::CreateTranslation(mPosition);
}

void Transform::LateUpdate() { }
