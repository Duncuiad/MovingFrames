#include "OpenGLPG_Base.h"

#include "CameraFactory.h"

#include "InputRotationCameraBehavior.h"
#include "InputTranslationCameraBehavior.h"

#include <glm/trigonometric.hpp>

Camera CameraFactory::CreateCamera(const CameraSettings& someSettings, const UID& aCameraUID)
{
    Camera newCamera {aCameraUID};

    // @note: in a normal production, this would be data-driven
    switch (someSettings.myType)
    {
    case FreeCam:
        AddCameraBehavior<InputRotationCameraBehavior>({glm::radians(50.f), glm::radians(50.f)}, newCamera);
        AddCameraBehavior<InputTranslationCameraBehavior>({5.f}, newCamera);
        break;
    case Orbital:
        AddCameraBehavior<InputRotationCameraBehavior>({glm::radians(50.f), glm::radians(50.f)}, newCamera);
        break;
    case Curve:
        break;
    }

    CameraRuntimeData& data {newCamera.myData};
    data.myFOV = someSettings.myFOV;
    data.myAspectRatio = someSettings.myAspectRatio;
    data.myNearPlane = someSettings.myNearPlane;
    data.myFarPlane = someSettings.myFarPlane;

    return newCamera;
}
