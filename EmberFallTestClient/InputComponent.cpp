#include "pch.h"
#include "InputComponent.h"
#include "Input.h"
#include "GameObject.h"

InputComponent::InputComponent() { }

InputComponent::~InputComponent() { }

void InputComponent::Update(const float deltaTime, GameObject& obj) {
    auto& transform = obj.GetTransform();
    auto speed = obj.GetSpeed();
    SimpleMath::Vector3 moveVec{ };
    SimpleMath::Vector3 look = transform.GetLook();
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
    if (Input::GetState(GLFW_MOUSE_BUTTON_2)) {
        /*glm::mat4 cameraRotate{ 1.f };

        cameraRotate = glm::rotate(cameraRotate, glm::radians(-deltaMouse.y * MOUSE_SENSITIVE), glm::vec3{ 1.0f, 0.0f, 0.0f });
        cameraRotate = glm::rotate(cameraRotate, glm::radians(-deltaMouse.x * MOUSE_SENSITIVE), glm::vec3{ 0.0f, 1.0f, 0.0f });*/

        deltaRotate.y = glm::radians(-deltaMouse.y * MOUSE_SENSITIVE);
        deltaRotate.x = glm::radians(-deltaMouse.x * MOUSE_SENSITIVE);
    }

    transform.Rotate(ConvertVec3(deltaRotate));
    transform.Translate(moveVec);
}

std::shared_ptr<Component> InputComponent::Clone() {
    return std::shared_ptr<Component>();
}
