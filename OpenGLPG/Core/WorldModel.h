#pragma once

#include "CameraData.h"
#include "CameraManager.h"
#include "Entity.h"
#include "UID.h"

class WorldModel
{
public:
    struct ConstructionParams
    {
        const ClientLoader& myClientLoader;
    };
    struct UpdateParams
    {
        const GameInputData& myGameInputData;
        float myDeltaTime;
    };

    WorldModel(const ConstructionParams& someParams);
    void Init();
    void Shutdown();
    void Update(const UpdateParams& someParams);

    bool IsAvailable() const;

    const CameraData& GetActiveCameraData() const;
    const Entity::Container& GetEntities() const { return myEntities; }

private:
    Entity::Container myEntities;

    CameraManager myCameraManager;
    UID myGameplayCamera {UID::Empty};
    const ClientLoader& myClientLoader;
};
