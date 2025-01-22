#pragma once

class GameObject;

class Component abstract {
public:
    Component() { }
    virtual ~Component() { }

public:
    virtual std::shared_ptr<Component> Clone() abstract;
    virtual void Update(const float deltaTime, GameObject& obj) abstract;

    template <typename ComponentT>
    static std::shared_ptr<ComponentT> CastComponent(const std::shared_ptr<Component>& component) {
        return std::static_pointer_cast<ComponentT>(component);
    }
};