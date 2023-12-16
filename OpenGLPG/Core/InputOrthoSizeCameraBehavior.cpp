#include "OpenGLPG_Base.h"

#include "InputOrthoSizeCameraBehavior.h"

#include "CameraRuntimeData.h"
#include "Serializer.h"

#include <cmath>

REGISTER_SUBTYPE(InputOrthoSizeCameraBehavior)

void InputOrthoSizeCameraBehavior::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("mySpeed", mySettings.mySpeed);
}

void InputOrthoSizeCameraBehavior::Update(const UpdateParams& someParams) const
{

    someParams.myCameraData.myOrthoSize *=
        exp(mySettings.mySpeed * -(someParams.myInputData.myMoveUD + someParams.myInputData.myLookUD));
}
