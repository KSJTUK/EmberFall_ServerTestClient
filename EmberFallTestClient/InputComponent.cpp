#include "pch.h"
#include "InputComponent.h"
#include "Input.h"
#include "GameObject.h"

InputComponent::InputComponent() { }

InputComponent::~InputComponent() { }

void InputComponent::Update(const float deltaTime, GameObject& obj) { 
}

std::shared_ptr<Component> InputComponent::Clone() {
    return std::shared_ptr<Component>();
}
