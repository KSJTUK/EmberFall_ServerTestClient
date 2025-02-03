#include "pch.h"
#include "InputComponent.h"
#include "Input.h"
#include "GameObject.h"

InputComponent::InputComponent() { }

InputComponent::~InputComponent() { }

void InputComponent::Update(const float deltaTime, GameObject& obj) {
    static float rotY{ };

    auto& transform = obj.GetTransform();
    auto speed = obj.GetSpeed();
    SimpleMath::Vector3 moveVec{ };
    if (Key::DOWN == Input::GetState(GLFW_KEY_W)) {
        moveVec.z -= speed * deltaTime;
    }

    if (Key::DOWN == Input::GetState(GLFW_KEY_S)) {
        moveVec.z += speed * deltaTime;
    }

    if (Key::DOWN == Input::GetState(GLFW_KEY_A)) {
        moveVec.x -= speed * deltaTime;
    }

    if (Key::DOWN == Input::GetState(GLFW_KEY_D)) {
        moveVec.x += speed * deltaTime;
    }

    if (Key::DOWN == Input::GetState(GLFW_KEY_E)) {
        moveVec.y -= speed * deltaTime;
    }

    if (Key::DOWN == Input::GetState(GLFW_KEY_Q)) {
        moveVec.y += speed * deltaTime;
    }

    glm::vec2 deltaMouse = Input::GetDeltaMouse();
    glm::vec3 deltaRotate{ 0.0f };
    SimpleMath::Quaternion cameraRotate{ SimpleMath::Quaternion::Identity };
    if (Input::GetState(GLFW_MOUSE_BUTTON_2)) {
        deltaRotate.x = glm::radians(-deltaMouse.x * MOUSE_SENSITIVE);
        deltaRotate.y = glm::radians(-deltaMouse.y * MOUSE_SENSITIVE);

        rotY += deltaRotate.y;
        if (rotY < -DirectX::XM_PIDIV2 + 0.5f or rotY > DirectX::XM_PIDIV2 - 0.5f) {
            std::cout << glm::degrees(rotY) << std::endl;
            rotY -= deltaRotate.y;
            deltaRotate.y = 0.0f;
        }

        cameraRotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::Up, deltaRotate.x);
        cameraRotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::Right, deltaRotate.y) * cameraRotate;
        if (std::fabs(deltaRotate.x) > DirectX::XM_PIDIV2) {
            std::cout << deltaRotate.x << std::endl;;
        }
    }

    transform.Move(moveVec);
    transform.Rotate(cameraRotate);
}

std::shared_ptr<Component> InputComponent::Clone() {
    return std::shared_ptr<Component>();
}
