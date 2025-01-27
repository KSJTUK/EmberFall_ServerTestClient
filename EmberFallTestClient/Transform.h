#pragma once

class Transform {
public:
    Transform();
    ~Transform();

    Transform(const Transform& other);
    Transform(Transform&& other) noexcept;
    Transform& operator=(const Transform& other);
    Transform& operator=(Transform&& other) noexcept;

public:
    glm::vec3 GetPosition() const;
    glm::vec3 GetRotation() const;
    glm::vec3 GetLook() const;
    glm::vec3 GetScale() const;
    glm::mat4 GetWorld() const;

    void MoveTo(const glm::vec3& toPos);
    void Move(const glm::vec3& moveVec);
    void Rotate(const glm::vec3& angles);
    void RotateTo(const glm::vec3& toVec);
    void Look(const glm::vec3& look);

    void Scale(const glm::vec3& scale);

    void Update();

private:
    glm::vec3 mLook{ 0.0f, 0.0f, -1.0f };
    glm::vec3 mPosition{ 0.0f };
    glm::vec3 mRotation{ 0.0f };
    glm::vec3 mScale{ 1.0f, 1.0f, 1.0f };

    glm::mat4 mWorld{ identity };
};

