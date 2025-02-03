#include "pch.h"
#include "TestComponent.h"
#include "GameObject.h"

TestComponent::TestComponent() { }

TestComponent::~TestComponent() { }

void TestComponent::Update(const float deltaTime, GameObject& obj) {
    auto& transform = obj.GetTransform();
    transform.Rotate(deltaTime, deltaTime, deltaTime);
}

std::shared_ptr<Component> TestComponent::Clone() {
    return std::shared_ptr<Component>();
}
