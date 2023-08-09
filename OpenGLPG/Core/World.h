#pragma once

#include "Array.h"
#include "CameraData.h"
#include "CameraManager.h"
#include "Entity.h"
#include "Filepath.h"
#include "UID.h"

class World
{
public:
    struct ConstructionParams
    {
        const ClientLoader& myClientLoader;
        Filepath aLevelName = "";
    };
    struct UpdateParams
    {
        const GameInputData& myGameInputData;
        float myDeltaTime;
    };

    World(const ConstructionParams& someParams);
    void Init();
    void Shutdown();
    void Update(const UpdateParams& someParams);
    void SaveWorld(const std::string& aLevelName);

    bool IsAvailable() const;
    UID RequestEntitySpawn(const Filepath& aTemplateFilepath);
    void RequestEntityUnspawn(const UID& anEntityUID);

    const CameraData& GetActiveCameraData() const;
    const Entity::Container& GetEntities() const { return myEntities; }
    const Entity& GetEntity(const UID& anEntityUID) const;
    Entity& GetEntity(const UID& anEntityUID);

private:
    void SpawnQueuedEntities();

    Entity::Container myEntities;
    Array<Entity> myEntityQueue;

    CameraManager myCameraManager;
    UID myGameplayCamera {UID::Empty};
    const ClientLoader& myClientLoader;
};
