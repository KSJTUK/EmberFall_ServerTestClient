#include "pch.h"
#include "InputComponent.h"
#include "Input.h"
#include "GameObject.h"

InputComponent::InputComponent() { }

InputComponent::~InputComponent() { }

void InputComponent::Update(const float deltaTime, GameObject& obj) {
    static float yaw{ 0.0f };
    static float pitch{ 0.0f };

    auto& transform = obj.GetTransform();

    SimpleMath::Vector2 deltaMouse = Input::GetDeltaMouse();
    SimpleMath::Vector2 deltaRotate{ 0.0f };
    if (Input::GetState(GLFW_MOUSE_BUTTON_2)) {
        SimpleMath::Quaternion cameraRotate{ SimpleMath::Quaternion::Identity };
        deltaRotate.x = glm::radians(-deltaMouse.x * MOUSE_SENSITIVE);
        deltaRotate.y = glm::radians(-deltaMouse.y * MOUSE_SENSITIVE);

        yaw += deltaRotate.x;
        pitch += deltaRotate.y;
        pitch = std::clamp(pitch,
            -DirectX::XM_PIDIV2 + 1.0f,
            DirectX::XM_PIDIV2 - 1.0f);

        cameraRotate = SimpleMath::Quaternion::CreateFromYawPitchRoll(yaw, 0.0f, 0.0f);
        //cameraRotate = SimpleMath::Quaternion::CreateFromAxisAngle(
        //    SimpleMath::Vector3::Up, rotAngle.x);
        //cameraRotate = SimpleMath::Quaternion::CreateFromAxisAngle(
        //    SimpleMath::Vector3::Right, rotAngle.y) * cameraRotate;

        cameraRotate.Normalize();
        transform.SetRotation(cameraRotate);
    }
}

std::shared_ptr<Component> InputComponent::Clone() {
    return std::shared_ptr<Component>();
}
