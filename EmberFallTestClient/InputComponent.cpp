#include "pch.h"
#include "InputComponent.h"
#include "Input.h"
#include "GameObject.h"

InputComponent::InputComponent() { }

InputComponent::~InputComponent() { }

void InputComponent::Update(const float deltaTime, GameObject& obj) {
    static SimpleMath::Vector2 rotAngle{ };

    auto& transform = obj.GetTransform();

    SimpleMath::Vector2 deltaMouse = Input::GetDeltaMouse();
    SimpleMath::Vector2 deltaRotate{ 0.0f };
    if (Input::GetState(GLFW_MOUSE_BUTTON_2)) {
        SimpleMath::Quaternion cameraRotate{ SimpleMath::Quaternion::Identity };
        deltaRotate.x = glm::radians(-deltaMouse.x * MOUSE_SENSITIVE);
        deltaRotate.y = glm::radians(-deltaMouse.y * MOUSE_SENSITIVE);

        rotAngle += deltaRotate;
        rotAngle.y = std::clamp(rotAngle.y, -DirectX::XM_PIDIV2 + 0.1f, DirectX::XM_PIDIV2 - 0.1f);

        cameraRotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::Up, rotAngle.x);
        cameraRotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::Right, rotAngle.y) * cameraRotate;
        cameraRotate.Normalize();
        transform.SetRotation(cameraRotate);
    }
}

std::shared_ptr<Component> InputComponent::Clone() {
    return std::shared_ptr<Component>();
}
