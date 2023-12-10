#pragma once

#include "Entity.h"

class CollisionSystem
{
public:
    struct ConstructionParams
    {
        const Entity::Container& myEntities;
    };
    CollisionSystem(const ConstructionParams& someParams);
    bool RayCast(const Vec3& aRayStart, const Vec3& aRayDirection) const;

private:
    const Entity::Container& myEntities;
};
