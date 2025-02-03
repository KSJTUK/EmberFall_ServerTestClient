#pragma once

#include "Component.h"

class TestComponent : public Component {
public:
    TestComponent();
    ~TestComponent();

public:
    virtual void Update(const float deltaTime, GameObject& obj) override;
    virtual std::shared_ptr<Component> Clone() override;
};