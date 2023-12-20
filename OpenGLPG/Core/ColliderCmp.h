#pragma once

#include "CollisionTag.h"
#include "Component.h"
#include "MathDefines.h"
#include "Serializer.h"
#include "TransformCmp.h"

#include <vector>

class ColliderCmp : public Component
{
public:
    virtual float RayCastHit(const Vec3& aRayStart, const Vec3& aRayDirection) const = 0;
    virtual void AddHit(const CollisionTag& aTag) const = 0;
    virtual void Reset() const {};
    bool RayVsBoundingSphere(const Vec3& aRayStart, const Vec3& aRayDirection) const;

    void SetBoundingSphere(float aRadius, const Vec3& aCenter);

protected:
    ColliderCmp();
    ColliderCmp(float aRadius, const Vec3& aCenter);

    void Serialize(Serializer& aSerializer) override;
    const TransformCmp* GetTransformCmp() const;

private:
    float myBoundingSphereRadius {0.f};
    Vec3 myBoundingSphereCenter {0.f};
};

template <typename CollisionData>
class ColliderCmpT : public ColliderCmp
{
public:
    void Reset() const override { myHits.clear(); }
    void AddHit(const CollisionTag& aTag) const override { myHits.emplace_back(aTag, myData); }

    struct Hit
    {
        CollisionTag myTag;
        CollisionData myData;
    };

    mutable std::vector<Hit> myHits;

protected:
    mutable CollisionData myData;
};
