#pragma once

class GameObject;

class Component abstract {
public:
    Component() : mOwner{ std::make_shared<GameObject>() } { }
    Component(std::shared_ptr<GameObject> owner) : mOwner{ owner } { }
    virtual ~Component() { }

public:
    std::shared_ptr<GameObject> GetOwner() { return mOwner; };
    virtual std::shared_ptr<Component> Clone() abstract;
    virtual void Update(const float deltaTime) abstract;

    template <typename ComponentT>
    static std::shared_ptr<ComponentT> CastComponent(const std::shared_ptr<Component>& component) {
        return std::static_pointer_cast<ComponentT>(component);
    }

private:
    std::shared_ptr<GameObject> mOwner{ };
};