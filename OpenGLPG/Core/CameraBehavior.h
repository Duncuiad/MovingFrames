#pragma once

#include "Array.h"
#include "CameraInputData.h"
#include "Serializable.h"

#include <memory>

struct CameraRuntimeData;

class CameraBehavior : public SerializableDynamic
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
    using Container = ArrayDynamic<UniquePtr>;
};

inline SerializableDynamic* Unwrap(const CameraBehavior::UniquePtr& aPtr)
{
    return aPtr.get();
}

template <>
inline CameraBehavior::UniquePtr Wrap<CameraBehavior::UniquePtr>(SerializableDynamic* aPtr)
{
    return CameraBehavior::UniquePtr {static_cast<CameraBehavior*>(aPtr)};
}
