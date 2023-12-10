#include "OpenGLPG_Base.h"

#include "CameraUtils.h"

#include "MathUtils.h"
#include "MovingFrame.h"

#include <cmath>

CameraData ComputeCameraData(const CameraRuntimeData& aCameraRuntimeData)
{
    const MovingFrame worldFrame {aCameraRuntimeData.myReferenceFrame * aCameraRuntimeData.myLocalFrame};
    return {Transform {Mat3 {worldFrame.GetOrientation()}, worldFrame.GetPosition()},
            aCameraRuntimeData.myFOV,
            2.f * atan(tan(aCameraRuntimeData.myFOV * 0.5f) / aCameraRuntimeData.myAspectRatio),
            aCameraRuntimeData.myAspectRatio,
            aCameraRuntimeData.myNearPlane,
            aCameraRuntimeData.myFarPlane,
            worldFrame};
}
