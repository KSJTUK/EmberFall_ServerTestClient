#include "pch.h"
#include "TestComponent.h"
#include "GameObject.h"

TestComponent::TestComponent() { }

TestComponent::~TestComponent() { }

void TestComponent::Update(const float deltaTime, GameObject& obj) {
    auto& transform = obj.GetTransform();
    transform.Rotate(deltaTime, 0.0f, 0.0f);
}

std::shared_ptr<Component> TestComponent::Clone() {
    return std::shared_ptr<Component>();
}
