#pragma once

#include "ColliderCmp.h"
#include "MathDefines.h"

class TileMesh;
class TileMeshCmp;
class TransformCmp;

struct TileMeshCollisionData
{
    int myHitFace {-1};
    int myHitVertex {-1};
};

class TileMeshColliderCmp : public ColliderCmpT<TileMeshCollisionData>
{
    DECLARE_SUBTYPE(TileMeshColliderCmp)
    using Base = ColliderCmpT<TileMeshCollisionData>;

public:
    void OnEnterWorld() override;

private:
    float RayCastHit(const Vec3& aRayStart, const Vec3& aRayDirection) const override;
    const TileMeshCmp& GetTileMeshCmp() const;
    const Transform& GetTransform() const;

    const TileMesh* myTileMesh {nullptr};
};
