#include "OpenGLPG_Base.h"

#include "InputTranslationCameraBehavior.h"

#include "CameraRuntimeData.h"

void InputTranslationCameraBehavior::Update(const UpdateParams& someParams) const
{
    const Vec3 moveVec {someParams.myInputData.myMoveRL, someParams.myInputData.myMoveUD,
                        -someParams.myInputData.myMoveFB};

    using namespace TwistFlags;
    Vec3& referencePosTwist =
        someParams.myCameraData.myDesiredTwist[Component::Translational | Coordinate::Intrinsic | Frame::Reference];

    referencePosTwist += mySettings.mySpeed * moveVec;
}
