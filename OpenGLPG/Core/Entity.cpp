#include "OpenGLPG_Base.h"

#include "Entity.h"

Entity::Entity() {}

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

void Entity::Spawn(const UID& anEntityUID)
{
    myUID = anEntityUID;
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

void Entity::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myComponents", myComponents);
}

const UID& Entity::GetUID()
{
    return myUID;
}
