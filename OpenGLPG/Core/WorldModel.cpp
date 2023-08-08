#include "OpenGLPG_Base.h"

#include "WorldModel.h"

#include "Assert.h"
#include "Serializer.h"

#include <utility>

WorldModel::WorldModel(const ConstructionParams& someParams)
    : myClientLoader {someParams.myClientLoader}
{
    if (someParams.aLevelName != "")
    {
        SerializerLoader loader(myClientLoader.myWorldFolder + someParams.aLevelName + ".world");
        myEntityQueue.Serialize(loader);
    }

    for (auto& entity : myEntityQueue)
    {
        entity.myUID = UID::CreateUnique();
        entity.Load({myClientLoader});
    }
}

void WorldModel::Init()
{
    SpawnQueuedEntities();

    // Temporary camera activation flow:

    // CameraActivationRequest request {{CameraType::FreeCam, glm::radians(45.f), 16.f / 9.f, 0.1f, 200.f}};
    // Keep this around in case I ever break the serialization

    CameraActivationRequest request {{myClientLoader.myAssetFolder + "Camera/freecam.camera"}};
    myGameplayCamera = request.GetCameraUID();
    myCameraManager.ActivateCamera(request);

    //
}

void WorldModel::Shutdown()
{
    myCameraManager.DeactivateCamera({myGameplayCamera});
}

void WorldModel::Update(const UpdateParams& someParams)
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

void WorldModel::SaveWorld(const std::string& aLevelName)
{
    ASSERT(!aLevelName.empty(), "Invalid name for asset!");
    SerializerSaver saver {myClientLoader.myWorldFolder + aLevelName};

    // @improvement: This needs to be kept in sync with the Array class deserialization parsing. Maybe let's make it
    // smell a bit less
    int entityCount {static_cast<int>(myEntities.size())};
    saver.Process("myCount", entityCount);

    // @note: I could use the uid instead of this counter. It wouldn't have much meaning to have uids in the level file
    // at the moment though
    int counter {0};
    for (auto& [uid, entity] : myEntities)
    {
        saver.Process(std::to_string(counter).data(), entity);
        ++counter;
    }
}

bool WorldModel::IsAvailable() const
{
    return true;
}

UID WorldModel::RequestEntitySpawn(const Filepath& aTemplateFilepath)
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

void WorldModel::RequestEntityUnspawn(const UID& anEntityUID)
{
    if (auto entity = myEntities.find(anEntityUID); entity != myEntities.end())
    {
        entity->second.Unspawn();
        myEntities.erase(entity);
    }
}

const CameraData& WorldModel::GetActiveCameraData() const
{
    return myCameraManager.GetActiveCameraData();
}

const Entity& WorldModel::GetEntity(const UID& anEntityUID) const
{
    ASSERT(myEntities.contains(anEntityUID), "Invalid Entity UID");
    return myEntities.at(anEntityUID);
}

Entity& WorldModel::GetEntity(const UID& anEntityUID)
{
    ASSERT(myEntities.contains(anEntityUID), "Invalid Entity UID");
    return myEntities.at(anEntityUID);
}

void WorldModel::SpawnQueuedEntities()
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
