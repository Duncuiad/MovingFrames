#include "OpenGLPG_Base.h"

#include "ColliderCmp.h"

#include "Assert.h"
#include "Entity.h"
#include "MathDefines.h"

bool ColliderCmp::RayVsBoundingSphere(const Vec3& aRayStart, const Vec3& aRayDirection) const
{
    using namespace glm;

    float radiusSqd {myBoundingSphereRadius * myBoundingSphereRadius};
    Vec3 center {myBoundingSphereCenter};
    if (const TransformCmp* transformCmp = GetTransformCmp())
    {
        const Transform& transform {transformCmp->GetTransform()};
        float scaleSqd {max(max(dot(transform[0], transform[0]), dot(transform[1], transform[1])),
                            dot(transform[2], transform[2]))};
        radiusSqd *= scaleSqd;
        center = Vec3(transform * Vec4(center, 1.f));
    }

    const Vec3 offset {center - aRayStart};
    float projected {max(dot(offset, aRayDirection), 0.f)};
    if (dot(offset, offset) - projected * projected / dot(aRayDirection, aRayDirection) < radiusSqd)
    {
        return true;
    }
    return false;
}

void ColliderCmp::SetBoundingSphere(float aRadius, const Vec3& aCenter)
{
    myBoundingSphereRadius = aRadius;
    myBoundingSphereCenter = aCenter;
}

ColliderCmp::ColliderCmp() {}

ColliderCmp::ColliderCmp(float aRadius, const Vec3& aCenter)
{
    SetBoundingSphere(aRadius, aCenter);
}

void ColliderCmp::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myBoundingSphereRadius", myBoundingSphereRadius);
    aSerializer.Process("myBoundingSphereCenter", myBoundingSphereCenter);
}

const TransformCmp* ColliderCmp::GetTransformCmp() const
{
    return GetEntity().GetComponent<TransformCmp>();
}
