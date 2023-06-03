#pragma once

#include "Camera.h"
#include "CameraData.h"
#include "CameraFactory.h"
#include "CameraRequests.h"
#include "CameraRuntimeData.h"
#include "GameInputData.h"

#include <vector>

class CameraManager
{
public:
    const CameraData& GetActiveCameraData() const { return myCameraData; }

    struct UpdateParams
    {
        const GameInputData& myGameInputData;
        float myDeltaTime;
    };
    void Update(const UpdateParams& someParams);

    void ActivateCamera(const CameraActivationRequest& aRequest);
    void DeactivateCamera(const CameraDeactivationRequest& aRequest);

private:
    CameraInputData GetInputData(const GameInputData& someGameInputData);
    void UpdateRuntimeData(CameraRuntimeData& anActiveCameraRuntimeData) const;

    using CameraContainer = std::vector<Camera>;

    CameraContainer myCameraContainer;
    CameraData myCameraData;
    CameraFactory myCameraFactory;
};
