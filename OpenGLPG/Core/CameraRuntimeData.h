#pragma once

#include "MovingFrame.h"

namespace TwistFlags
{
enum Component {
    Rotational = 0u,
    Translational = 1u << 0
};
enum Coordinate {
    Extrinsic = 0u,
    Intrinsic = 1u << 1
};
enum Frame {
    Reference = 0u,
    Local = 1u << 2
};
} // namespace TwistFlags

struct CameraRuntimeData
{
    MovingFrame myReferenceFrame {};
    MovingFrame myLocalFrame {};
    bool myIsOrtho {false};
    float myAspectRatio {0.f};
    float myNearPlane {0.f};
    float myFarPlane {0.f};

    mutable float myFOV {0.f};
    mutable float myOrthoSize {0.f};
    mutable Vec3 myDesiredTwist[8];
};
