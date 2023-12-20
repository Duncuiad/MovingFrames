#pragma once

#include "CollisionTag.h"
#include "Entity.h"

class CollisionSystem
{
public:
    struct ConstructionParams
    {
        const Entity::Container& myEntities;
    };
    CollisionSystem(const ConstructionParams& someParams);
    bool RayCast(const Vec3& aRayStart, const Vec3& aRayDirection, const CollisionTag& aTag = CollisionTag::None) const;

private:
    const Entity::Container& myEntities;
};
