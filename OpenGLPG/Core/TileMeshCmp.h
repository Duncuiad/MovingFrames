#pragma once

#include "Component.h"
#include "TileMesh.h"

class TileMeshCmp : public Component
{
    DECLARE_SUBTYPE(TileMeshCmp)

public:
    void Serialize(Serializer& aSerializer) override;

    TileMesh myTileMesh;
};
