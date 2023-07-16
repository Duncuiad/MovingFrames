#include "OpenGLPG_Base.h"

#include "Component.h"

#include "Entity.h"
#include "WorldModel.h"

void Component::EnterWorld(const UID& anEntityUID, WorldModel* aWorldModel)
{
    myEntity = anEntityUID;
    myWorld = aWorldModel;

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
