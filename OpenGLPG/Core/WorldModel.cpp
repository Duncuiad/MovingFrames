#include "OpenGLPG_Base.h"

#include "WorldModel.h"

WorldModel::WorldModel(const ConstructionParams& someParams)
    : myClientLoader {someParams.myClientLoader}
{}

void WorldModel::Init()
{
    //

    UID newUID {UID::CreateUnique()};
    // Entity newEntity {newUID};
    // myEntities.try_emplace(newUID, std::move(newEntity));
    myEntities.try_emplace(newUID, newUID);

    for (auto& [uid, entity] : myEntities)
    {
        entity.Load({myClientLoader});
    }

    //
}

void WorldModel::Shutdown() {}

void WorldModel::Update()
{
    for (auto& [uid, entity] : myEntities)
    {
        entity.Update();
    }
}

bool WorldModel::IsAvailable() const
{
    return true;
}

const CameraData& WorldModel::GetActiveCameraData()
{
    CameraData data;
    data.myCameraTransform = {};
    data.myFOV = glm::radians(45.f);
    data.myNear = 0.5f;
    data.myFar = 100.f;
    data.myAspectRatio = 16.f / 9.f;
    return data;
}
