#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Transform.cpp
// 2025 - 02 - 01 김성준 : 게임 오브젝트의 변환 행렬 표현을 위한 클래스 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Transform {
public:
    Transform();
    ~Transform();

    Transform(const Transform& other);
    Transform(Transform&& other) noexcept;
    Transform& operator=(const Transform& other);
    Transform& operator=(Transform&& other) noexcept;

public:
    SimpleMath::Vector3 GetRight() const;
    SimpleMath::Vector3 GetLook() const;
    SimpleMath::Vector3 GetUp() const;
    SimpleMath::Vector3 GetPosition() const;
    SimpleMath::Quaternion GetRotation() const;
    SimpleMath::Vector3 GetScale() const;
    SimpleMath::Matrix GetWorld() const;

    void SetPosition(const SimpleMath::Vector3& pos);

    void Translate(const SimpleMath::Vector3& v);
    void Move(const SimpleMath::Vector3& moveVec);

    void Rotate(const float yaw = 0.0f, const float pitch = 0.0f, const float roll = 0.0f);
    void Rotate(const SimpleMath::Vector3& v);
    void Rotate(const SimpleMath::Quaternion& quat);
    void RotateSmoothly(const SimpleMath::Quaternion& quat);

    void Scale(const SimpleMath::Vector3& v);

    void Update();

private:
    SimpleMath::Vector3 mPosition{ SimpleMath::Vector3::Zero };
    SimpleMath::Quaternion mRotation{ SimpleMath::Quaternion::Identity };
    SimpleMath::Vector3 mScale{ SimpleMath::Vector3::One };

    SimpleMath::Matrix mWorld{ SimpleMath::Matrix::Identity };
};
