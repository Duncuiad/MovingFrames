#include "OpenGLPG_Base.h"

#include "ColliderCmp.h"

#include "Assert.h"
#include "Entity.h"

bool ColliderCmp::RayVsBoundingSphere(const Vec3& aRayStart, const Vec3& aRayDirection) const
{
    ASSERT(false, "Method not implemented!");
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
