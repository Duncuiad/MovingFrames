#include "OpenGLPG_Base.h"

#include "CameraUtils.h"

#include "MathUtils.h"
#include "MovingFrame.h"

CameraData ComputeCameraData(const CameraRuntimeData& aCameraRuntimeData)
{
    const MovingFrame worldFrame {aCameraRuntimeData.myReferenceFrame * aCameraRuntimeData.myLocalFrame};
    return {Transform {Mat3 {worldFrame.GetOrientation()}, worldFrame.GetPosition()},
            aCameraRuntimeData.myFOV,
            aCameraRuntimeData.myAspectRatio,
            aCameraRuntimeData.myNearPlane,
            aCameraRuntimeData.myFarPlane,
            worldFrame};
}
