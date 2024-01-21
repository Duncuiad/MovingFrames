#pragma once

#include "Filepath.h"
#include "MathDefines.h"
#include "Serializable.h"

#include <optional>

class WorldSettings : public Serializable
{
public:
    void Serialize(Serializer& aSerializer) override;

    Filepath myCameraAsset;
    std::optional<Vec3> myBackgroundColor;
};
