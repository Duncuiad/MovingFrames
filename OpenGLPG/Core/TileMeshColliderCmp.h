#pragma once

#include "ColliderCmp.h"
#include "MathDefines.h"
#include "TileMeshCmp.h"

struct TileMeshCollisionData
{
    int myHitFace {-1};
};

class TileMeshColliderCmp : public ColliderCmpT<TileMeshCollisionData>
{
    DECLARE_SUBTYPE(TileMeshColliderCmp)
    using Base = ColliderCmpT<TileMeshCollisionData>;

public:
    TileMeshColliderCmp();

private:
    virtual bool RayCastHit(const Vec3& aRayStart, const Vec3& aRayDirection) const override;
    const TileMeshCmp& GetTileMeshCmp() const;

    const TileMesh* myTileMesh {nullptr};
};
