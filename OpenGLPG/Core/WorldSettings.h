#pragma once

#include "Filepath.h"
#include "Serializable.h"

class WorldSettings : public Serializable
{
public:
    void Serialize(Serializer& aSerializer) override;

    Filepath myCameraAsset;
};
