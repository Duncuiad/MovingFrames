#include "OpenGLPG_Base.h"

#include "Entity.h"

Entity::Entity() {}

void Entity::Load(const LoadParams& someParams)
{
    for (const auto& component : myComponents)
    {
        component->OnLoad({someParams.myClientLoader});
    }
}

void Entity::Spawn(WorldModel* aWorldModel)
{
    for (auto& component : myComponents)
    {
        component->EnterWorld(myUID, aWorldModel);
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
        component->ExitWorld();
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
