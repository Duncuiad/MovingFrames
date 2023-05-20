#include "OpenGLPG_Base.h"

#include "WorldModel.h"

void WorldModel::Init() {}

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
