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
    gMouse = false;
    if (Input::GetState(GLFW_MOUSE_BUTTON_2)) {
        gMouse = true;
        SimpleMath::Quaternion cameraRotate{ SimpleMath::Quaternion::Identity };
        deltaRotate.x = glm::radians(-deltaMouse.x * MOUSE_SENSITIVE);
        deltaRotate.y = glm::radians(-deltaMouse.y * MOUSE_SENSITIVE);

        yaw += deltaRotate.x;
        if (yaw < 0.0f) {
            yaw += DirectX::XM_2PI;
        }

        cameraRotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::Up, yaw);

        cameraRotate.Normalize();
        transform.SetRotation(cameraRotate);

        gLogConsole->PushLog(DebugLevel::LEVEL_INFO, "Yaw: {}", yaw);
    }
}

std::shared_ptr<Component> InputComponent::Clone() {
    return std::shared_ptr<Component>();
}
