#include "OpenGLPG_Base.h"

#include "Entity.h"

// @temp: remove once you can spawn entities
#include "GraphCmp.h"

Entity::Entity(const UID& aUID)
    : myUID {aUID}
{
    // @temp: remove explicit components from Entity
    myComponents.emplace_back(new GraphCmp("backpack/backpack.obj", "basic.shader"));
}

/*
Entity::Entity(Entity&& anEntity) noexcept
    : myUID {anEntity.myUID}
{
    myComponents = std::move(anEntity.myComponents);
}
*/

void Entity::Load(const LoadParams& someParams)
{
    for (const auto& component : myComponents)
    {
        component->OnLoad({someParams.myClientLoader});
    }
}

void Entity::Spawn()
{
    for (const auto& component : myComponents)
    {
        component->OnEnterWorld();
    }
}

void Entity::Update()
{
    for (const auto& component : myComponents)
    {
        component->Update();
    }
}

void Entity::Unspawn()
{
    for (const auto& component : myComponents)
    {
        component->OnExitWorld();
    }
}

void Entity::Unload()
{
    for (const auto& component : myComponents)
    {
        component->OnUnload();
    }
}

const UID& Entity::GetUID()
{
    return myUID;
}
