#pragma once

#include "CameraData.h"
#include "Entity.h"
#include "UID.h"

class WorldModel
{
public:
    WorldModel() = default;
    void Init();
    void Shutdown();
    void Update();

    bool IsAvailable() const;

    const CameraData& GetActiveCameraData();
    const EntityContainer& GetEntities() { return myEntities; }

private:
    EntityContainer myEntities;
};
