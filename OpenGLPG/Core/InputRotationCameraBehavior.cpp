#include "OpenGLPG_Base.h"

#include "InputRotationCameraBehavior.h"

#include "CameraRuntimeData.h"

void InputRotationCameraBehavior::Update(const UpdateParams& someParams) const
{
    const float lookLR {someParams.myInputData.myLookRL};
    const float lookUD {someParams.myInputData.myLookUD};

    using namespace TwistFlags;
    Vec3& azimuthTwist =
        someParams.myCameraData.myDesiredTwist[Component::Rotational | Coordinate::Extrinsic | Frame::Reference];
    Vec3& zenithTwist =
        someParams.myCameraData.myDesiredTwist[Component::Rotational | Coordinate::Intrinsic | Frame::Reference];

    constexpr Vec3 upVector {0.f, 1.f, 0.f};
    azimuthTwist += lookLR * mySettings.myHorizontalSpeed * upVector;

    constexpr Vec3 rightVector {1.f, 0.f, 0.f};
    const Vec3 localRightVector {someParams.myCameraData.myLocalFrame.GetOrientation() * rightVector};
    zenithTwist += lookUD * mySettings.myVerticalSpeed * localRightVector;
}
