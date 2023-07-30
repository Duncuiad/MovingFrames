#pragma once

#include "Array.h"
#include "Component.h"
#include "Serializable.h"
#include "UID.h"

#include <unordered_map>

class WorldModel;

class Entity : public Serializable
{
public:
    using Container = std::unordered_map<UID, Entity>;

    struct LoadParams
    {
        const ClientLoader& myClientLoader;
    };

    Entity();
    Entity(const Entity& anEntity) = delete;
    Entity(Entity&& anEntity) noexcept = default;
    Entity& operator=(const Entity& anEntity) = delete;
    Entity& operator=(Entity&& anEntity) noexcept = default;

    void Load(const LoadParams& someParams);
    void Spawn(WorldModel* aWorldModel);
    void Update();
    void Unspawn();
    void Unload();

    void Serialize(Serializer& aSerializer) override;

    const UID& GetUID();

    template <typename ComponentT>
    const ComponentT* GetComponent() const;
    template <typename ComponentT>
    ComponentT* GetEditableComponent() const;
    template <typename ComponentT>
    std::vector<const ComponentT*> GetComponents() const;
    template <typename ComponentT>
    std::vector<ComponentT*> GetEditableComponents() const;

private:
    UID myUID;
    ArrayDynamic<Component::Ptr> myComponents;

    friend class WorldModel;
    friend struct std::pair<const UID, Entity>;
};

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

template <typename ComponentT>
inline std::vector<const ComponentT*> Entity::GetComponents() const
{
    std::vector<const ComponentT*> components;
    for (const auto& component : myComponents)
    {
        if (ComponentT* derivedComponent = dynamic_cast<ComponentT*>(component.get()))
        {
            components.push_back(derivedComponent);
        }
    }
    return components;
}

template <typename ComponentT>
inline std::vector<ComponentT*> Entity::GetEditableComponents() const
{
    std::vector<ComponentT*> components;
    for (const auto& component : myComponents)
    {
        if (ComponentT* derivedComponent = dynamic_cast<ComponentT*>(component.get()))
        {
            components.push_back(derivedComponent);
        }
    }
    return components;
}
