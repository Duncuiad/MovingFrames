#include "OpenGLPG_Base.h"

#include "CameraFactory.h"

#include "Assert.h"
#include "InputRotationCameraBehavior.h"
#include "InputTranslationCameraBehavior.h"
#include "Serializer.h"

#include <glm/trigonometric.hpp>

Camera CameraFactory::CreateCamera(const CameraSettings& someSettings, const UID& aCameraUID)
{
    Camera newCamera {aCameraUID};

    if (someSettings.myType == CameraType::FileBased)
    {
        ASSERT(someSettings.myAsset.HasExtension("camera"), "File has the wrong extension");
        SerializerLoader loader {someSettings.myAsset};
        newCamera.Serialize(loader);
    }
    else
    {
        switch (someSettings.myType)
        {
        case FreeCam:
            AddCameraBehavior<InputRotationCameraBehavior>({glm::radians(50.f), glm::radians(50.f)}, newCamera);
            AddCameraBehavior<InputTranslationCameraBehavior>({1.f}, newCamera);
            break;
        case Orbital:
            AddCameraBehavior<InputRotationCameraBehavior>({glm::radians(50.f), glm::radians(50.f)}, newCamera);
            break;
        case Curve:
            break;
        default:
            break;
        }

        CameraRuntimeData& data {newCamera.myData};
        data.myFOV = someSettings.myFOV;
        data.myAspectRatio = someSettings.myAspectRatio;
        data.myNearPlane = someSettings.myNearPlane;
        data.myFarPlane = someSettings.myFarPlane;
    }

    return newCamera;
}
