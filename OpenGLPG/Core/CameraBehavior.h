#pragma once

#include "CameraInputData.h"

#include <memory>
#include <vector>

struct CameraRuntimeData;

class CameraBehavior
{
public:
    struct Settings
    {};

    virtual ~CameraBehavior() = default;
    // CameraBehavior(const CameraBehavior&) = delete;
    // CameraBehavior(CameraBehavior&&) = default;

    struct UpdateParams
    {
        const CameraRuntimeData& myCameraData;
        const CameraInputData& myInputData;
        float myDeltaTime;
    };
    virtual void Update(const UpdateParams& /*someParams*/) const = 0;

    using UniquePtr = std::unique_ptr<CameraBehavior>;
    using Container = std::vector<UniquePtr>;
};
