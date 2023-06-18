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

    CameraActivationRequest request {{CameraType::FreeCam, glm::radians(45.f), 16.f / 9.f, 0.1f, 200.f}};
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
    for (auto& [uid, entity] : myEntities)
    {
        entity.Update();
    }

    myCameraManager.Update({someParams.myGameInputData, someParams.myDeltaTime});
}

bool WorldModel::IsAvailable() const
{
    return true;
}

const CameraData& WorldModel::GetActiveCameraData() const
{
    return myCameraManager.GetActiveCameraData();
}
