#pragma once

#include "Camera.h"
#include "CameraSettings.h"
#include "UID.h"

#include <type_traits>

class CameraFactory
{
public:
    Camera CreateCamera(const CameraSettings& someSettings, const UID& aCameraUID);

    // @note: this isn't static because different worlds may need different camera factories with specialized data
};

template <typename BehaviorT>
void AddCameraBehavior(const typename BehaviorT::Settings& someSettings, Camera& outCamera)
{
    static_assert(std::is_base_of_v<CameraBehavior, BehaviorT>, "Selected type is not a CameraBehavior");
    CameraBehavior::UniquePtr newBehavior {std::make_unique<BehaviorT>(someSettings)};
    outCamera.myBehaviors.EmplaceBack(std::move(newBehavior));
}
