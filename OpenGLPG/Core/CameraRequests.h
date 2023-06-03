#pragma once

#include "CameraSettings.h"
#include "UID.h"

struct CameraActivationRequest
{
    CameraActivationRequest(const CameraSettings& someSettings);

    [[nodiscard]] const UID& GetCameraUID() const { return myCameraUID; }

    CameraSettings mySettings;

private:
    UID myCameraUID;
};

struct CameraDeactivationRequest
{
    UID myCameraUID;
};
