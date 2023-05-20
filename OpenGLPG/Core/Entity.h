#pragma once

#include "Component.h"
#include "UID.h"

#include <unordered_map>
#include <vector>

class Entity
{
public:
    Entity();
    void Load();
    void Spawn();
    void Update();
    void Unspawn();
    void Unload();

    const UID& GetUID();

    template <typename ComponentT>
    const ComponentT* GetComponent() const;
    template <typename ComponentT>
    ComponentT* GetEditableComponent() const;

private:
    UID myUID;
    std::vector<Component::Ptr> myComponents;
};

using EntityContainer = std::unordered_map<UID, Entity>;

template <typename ComponentT>
inline const ComponentT* Entity::GetComponent() const
{
    const ComponentT* derivedComponent {GetEditableComponent<ComponentT>()};
    return derivedComponent;
}

template <typename ComponentT>
inline ComponentT* Entity::GetEditableComponent() const
{
    // @improvement: maybe use a hashmap to find components faster. This means I'd need to register component types to
    // give them a UID (or create a ClassID structure)
    for (const auto& component : myComponents)
    {
        if (ComponentT* derivedComponent = dynamic_cast<ComponentT*>(component.get()))
        {
            return derivedComponent;
        }
    }
    return nullptr;
}
