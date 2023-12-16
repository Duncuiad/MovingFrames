#include "OpenGLPG_Base.h"

#include "CameraUtils.h"

#include "MathUtils.h"
#include "MovingFrame.h"

#include <cmath>

CameraData ComputeCameraData(const CameraRuntimeData& aCameraRuntimeData)
{
    const MovingFrame worldFrame {aCameraRuntimeData.myReferenceFrame * aCameraRuntimeData.myLocalFrame};
    return {Transform {Mat3 {worldFrame.GetOrientation()}, worldFrame.GetPosition()},
            aCameraRuntimeData.myIsOrtho ? 0.f : aCameraRuntimeData.myFOV,
            aCameraRuntimeData.myIsOrtho
                ? 0.f
                : 2.f * atan(tan(aCameraRuntimeData.myFOV * 0.5f) / aCameraRuntimeData.myAspectRatio),
            aCameraRuntimeData.myIsOrtho ? aCameraRuntimeData.myOrthoSize : 0.f,
            aCameraRuntimeData.myAspectRatio,
            aCameraRuntimeData.myNearPlane,
            aCameraRuntimeData.myFarPlane,
            worldFrame,
            aCameraRuntimeData.myIsOrtho};
}
