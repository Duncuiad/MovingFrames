#pragma once

#include "Transform.h"

struct CameraData
{
    Transform myCameraTransform;
    float myFOV;
    float myAspectRatio;
    float myNear;
    float myFar;
};
