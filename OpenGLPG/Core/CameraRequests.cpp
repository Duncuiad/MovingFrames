#include "OpenGLPG_Base.h"

#include "CameraRequests.h"

CameraActivationRequest::CameraActivationRequest(const CameraSettings& someSettings)
    : mySettings {someSettings}
    , myCameraUID {UID::CreateUnique()}
{}
