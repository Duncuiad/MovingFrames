#include "OpenGLPG_Base.h"

#include "World.h"

#include "Assert.h"
#include "Serializer.h"

#include <utility>

World::World(const ConstructionParams& someParams)
    : myClientLoader {someParams.myClientLoader}
    , myCollisionSystem {{myEntities}}
{
    myClientLoader.GetWorldLoader().LoadWorld(someParams.aLevelName, this);

    for (auto& entity : myEntityQueue)
    {
        entity.myUID = UID::CreateUnique();
        entity.Load({myClientLoader});
    }
}

void World::Init()
{
    SpawnQueuedEntities();

    if (myWorldSettings.myCameraAsset == "")
    {
        myWorldSettings.myCameraAsset = "freecam.camera";
    }
    CameraActivationRequest request {{myClientLoader.myAssetFolder + "Camera/" + myWorldSettings.myCameraAsset}};
    myGameplayCamera = request.GetCameraUID();
    myCameraManager.ActivateCamera(request);
}

void World::Shutdown()
{
    myCameraManager.DeactivateCamera({myGameplayCamera});
}

void World::Update(const UpdateParams& someParams)
{
    for (auto& entity : myEntityQueue)
    {
        entity.Load({myClientLoader});
    }
    SpawnQueuedEntities();

    for (auto& [uid, entity] : myEntities)
    {
        entity.Update();
    }

    myCameraManager.Update({someParams.myGameInputData, someParams.myDeltaTime});
}

void World::SaveWorld(const std::string& aLevelName)
{
    myClientLoader.GetWorldLoader().SaveWorld(aLevelName, this);
}

bool World::IsAvailable() const
{
    return true;
}

UID World::RequestEntitySpawn(const Filepath& aTemplateFilepath)
{
    ASSERT(aTemplateFilepath.HasExtension("template"), "Wrong file extension");
    Entity newEntity;
    SerializerLoader loader(aTemplateFilepath);
    newEntity.Serialize(loader);
    const UID newUID {UID::CreateUnique()};
    newEntity.myUID = newUID;
    myEntityQueue.EmplaceBack(std::move(newEntity));
    return newEntity.myUID;
}

void World::RequestEntityUnspawn(const UID& anEntityUID)
{
    if (auto entity = myEntities.find(anEntityUID); entity != myEntities.end())
    {
        entity->second.Unspawn();
        myEntities.erase(entity);
    }
}

const CameraData& World::GetActiveCameraData() const
{
    return myCameraManager.GetActiveCameraData();
}

const Entity& World::GetEntity(const UID& anEntityUID) const
{
    ASSERT(myEntities.contains(anEntityUID), "Invalid Entity UID");
    return myEntities.at(anEntityUID);
}

Entity& World::GetEntity(const UID& anEntityUID)
{
    ASSERT(myEntities.contains(anEntityUID), "Invalid Entity UID");
    return myEntities.at(anEntityUID);
}

void World::SpawnQueuedEntities()
{
    for (auto& entity : myEntityQueue)
    {
        const UID entityUID {entity.myUID};
        auto pair {myEntities.try_emplace(entityUID, std::move(entity))};
        Entity& newEntity {pair.first->second};
        newEntity.Spawn(this);
    }
    myEntityQueue.RemoveAll();
}
