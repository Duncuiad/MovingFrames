#pragma once

#include "MovingFrame.h"
#include "Transform.h"

struct CameraData
{
    Transform myCameraTransform;
    float myFOV;
    float myAspectRatio;
    float myNear;
    float myFar;
    MovingFrame myWorldFrame;
};
