#pragma once

#include "InputComponent.h"

class GameObject {
public:
    GameObject() { }  
    ~GameObject() { }

public:
    void Update(const float deltaTime);

private:
    InputComponent mInputComponent{ };
};