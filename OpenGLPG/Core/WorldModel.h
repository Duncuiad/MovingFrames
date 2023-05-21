#pragma once

#include "CameraData.h"
#include "Entity.h"
#include "UID.h"

class WorldModel
{
public:
    struct ConstructionParams
    {
        const ClientLoader& myClientLoader;
    };

    WorldModel(const ConstructionParams& someParams);
    void Init();
    void Shutdown();
    void Update();

    bool IsAvailable() const;

    const CameraData& GetActiveCameraData();
    const Entity::Container& GetEntities() const { return myEntities; }

private:
    Entity::Container myEntities;

    const ClientLoader& myClientLoader;
};
