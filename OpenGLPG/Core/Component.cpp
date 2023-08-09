#include "OpenGLPG_Base.h"

#include "Component.h"

#include "Entity.h"
#include "World.h"

void Component::EnterWorld(const UID& anEntityUID, World* aWorld)
{
    myEntity = anEntityUID;
    myWorld = aWorld;

    OnEnterWorld();
}

void Component::ExitWorld()
{
    OnExitWorld();

    myEntity = UID::Empty;
    myWorld = nullptr;
}

Entity& Component::GetEntity() const
{
    ASSERT(myEntity != UID::Empty && myWorld != nullptr, "Entity isn't spawned");
    return myWorld->GetEntity(myEntity);
}
