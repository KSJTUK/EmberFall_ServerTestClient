#pragma once

#include "Component.h"
#include <functional>

class InputComponent : public Component {
public:
    InputComponent();
    ~InputComponent();

public:
    virtual void Update(const float deltaTime, GameObject& obj) override;
    virtual std::shared_ptr<Component> Clone() override;
};