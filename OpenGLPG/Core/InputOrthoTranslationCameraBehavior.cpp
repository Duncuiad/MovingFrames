#include "OpenGLPG_Base.h"

#include "InputOrthoTranslationCameraBehavior.h"

#include "CameraRuntimeData.h"
#include "Serializer.h"

REGISTER_SUBTYPE(InputOrthoTranslationCameraBehavior)

void InputOrthoTranslationCameraBehavior::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("mySpeed", mySettings.mySpeed);
}

void InputOrthoTranslationCameraBehavior::Update(const UpdateParams& someParams) const
{
    const Vec3 moveVec {someParams.myInputData.myMoveRL, someParams.myInputData.myMoveFB, 0.f};

    using namespace TwistFlags;
    Vec3& referencePosTwist =
        someParams.myCameraData.myDesiredTwist[Component::Translational | Coordinate::Intrinsic | Frame::Reference];

    referencePosTwist += mySettings.mySpeed * moveVec * someParams.myCameraData.myOrthoSize;
}
