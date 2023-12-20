#include "OpenGLPG_Base.h"

#include "CollisionSystem.h"

#include "Assert.h"
#include "ColliderCmp.h"

#include <glm/geometric.hpp>
#include <optional>

CollisionSystem::CollisionSystem(const ConstructionParams& someParams)
    : myEntities {someParams.myEntities}
{}

bool CollisionSystem::RayCast(const Vec3& aRayStart, const Vec3& aRayDirection, const CollisionTag& aTag) const
{
    ASSERT(glm::length(aRayDirection) > 0.f, "Invalid ray cast direction");
    const Vec3 direction {glm::normalize(aRayDirection)};
    const ColliderCmp* hitCollider {nullptr};
    std::optional<float> minDistance;
    for (const auto& pair : myEntities)
    {
        const ColliderCmp* colliderPtr {pair.second.GetComponent<ColliderCmp>()};
        if (colliderPtr != nullptr && colliderPtr->RayVsBoundingSphere(aRayStart, direction))
        {
            const float hitDistance {colliderPtr->RayCastHit(aRayStart, direction)};
            if (hitDistance >= 0.f && (!minDistance.has_value() || hitDistance < *minDistance))
            {
                minDistance = hitDistance;
                hitCollider = colliderPtr;
            }
        }
    }

    if (hitCollider != nullptr)
    {
        hitCollider->AddHit(aTag);
        return true;
    }
    return false;
}
