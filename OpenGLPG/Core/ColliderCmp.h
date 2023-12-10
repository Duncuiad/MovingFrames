#pragma once

#include "Component.h"
#include "MathDefines.h"
#include "Serializer.h"
#include "TransformCmp.h"

class ColliderCmp : public Component
{
public:
    virtual bool RayCastHit(const Vec3& aRayStart, const Vec3& aRayDirection) const = 0;
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
    void Reset() const override { myData = CollisionData {}; }

protected:
    mutable CollisionData myData;
};
