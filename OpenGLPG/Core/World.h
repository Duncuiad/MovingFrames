#pragma once

#include "Array.h"
#include "CameraData.h"
#include "CameraManager.h"
#include "CollisionSystem.h"
#include "Entity.h"
#include "Filepath.h"
#include "UID.h"
#include "WorldSettings.h"

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

    const CollisionSystem& GetCollisionSystem() const { return myCollisionSystem; }
    const CameraData& GetActiveCameraData() const;
    const Entity::Container& GetEntities() const { return myEntities; }
    const Entity& GetEntity(const UID& anEntityUID) const;
    Entity& GetEntity(const UID& anEntityUID);

private:
    void SpawnQueuedEntities();

    friend class WorldLoader;

    WorldSettings myWorldSettings;
    Entity::Container myEntities;
    Array<Entity> myEntityQueue;

    CollisionSystem myCollisionSystem;
    CameraManager myCameraManager;
    UID myGameplayCamera {UID::Empty};
    const ClientLoader& myClientLoader;
};
