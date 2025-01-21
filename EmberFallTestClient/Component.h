#pragma once
#include "GameObject.h"

template <typename T>
class Component abstract {
public:
    Component() : mOwner{ std::make_shared<GameObject>() } { }
    Component(std::shared_ptr<T> owner) : mOwner{ owner } { }
    virtual ~Component() { }

public:
    std::shared_ptr<T> GetOwner() { return mOwner; };
    virtual std::shared_ptr<Component> Clone() abstract;
    virtual void Update(const float deltaTime) abstract;

    template <typename ComponentT>
    static std::shared_ptr<ComponentT> CastComponent(std::shared_ptr<Component>& component) {
        return std::static_pointer_cast<ComponentT>(component);
    }

private:
    std::shared_ptr<T> mOwner{ };
};

class TestComponent : public Component<GameObject> {
public:
    virtual void Update(const float deltaTime) override { 
        
    }

    virtual std::shared_ptr<Component> Clone() override {
        return std::make_shared<TestComponent>();
    }
};