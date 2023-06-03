#pragma once

#include "CameraBehavior.h"
#include "CameraRuntimeData.h"
#include "MovingFrame.h"
#include "UID.h"

class Camera
{
public:
    // @note: Cameras may need to be moved around in a camera container. However, CameraBehaviors are non-copyable.
    // Hence the need for explicit move ctor and assignment operator
    Camera(Camera&&) = default;
    Camera& operator=(Camera&&) = default;

    UID myUID;
    CameraRuntimeData myData;
    CameraBehavior::Container myBehaviors;

private:
    // @note: hidden constructor
    explicit Camera(const UID& aCameraUID);
    friend class CameraFactory;
};
