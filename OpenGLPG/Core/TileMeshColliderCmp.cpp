#include "OpenGLPG_Base.h"

#include "TileMeshColliderCmp.h"

#include "Assert.h"
#include "Entity.h"
#include "TileMesh.h"

REGISTER_SUBTYPE(TileMeshColliderCmp)

TileMeshColliderCmp::TileMeshColliderCmp()
    : Base {}
{
    myTileMesh = &GetTileMeshCmp().myTileMesh;
}

bool TileMeshColliderCmp::RayCastHit(const Vec3& aRayStart, const Vec3& aRayDirection) const
{
    ASSERT(false, "Method not implemented!");
    return false;
}

const TileMeshCmp& TileMeshColliderCmp::GetTileMeshCmp() const
{
    const TileMeshCmp* cmp {GetEntity().GetComponent<TileMeshCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return *cmp;
}
