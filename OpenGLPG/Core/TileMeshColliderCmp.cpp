#include "OpenGLPG_Base.h"

#include "TileMeshColliderCmp.h"

#include "Assert.h"
#include "Entity.h"
#include "MathDefines.h"
#include "TileMesh.h"
#include "TileMeshCmp.h"
#include "Transform.h"
#include "TransformCmp.h"

#include <utility>

REGISTER_SUBTYPE(TileMeshColliderCmp)

void TileMeshColliderCmp::OnEnterWorld()
{
    myTileMesh = &GetTileMeshCmp().myTileMesh;
}

float TileMeshColliderCmp::RayCastHit(const Vec3& aRayStart, const Vec3& aRayDirection) const
{
    ASSERT(myTileMesh != nullptr, "Invalid TileMesh");

    const Mat4 inverseTransform {glm::affineInverse(GetTransform())};
    const Vec3 rayStart {inverseTransform * Vec4(aRayStart, 1.f)};
    const Vec3 rayDirection {Mat3(inverseTransform) * aRayDirection};

    if (rayDirection.z != 0.f)
    {
        const float distance {-rayStart.z / rayDirection.z};
        if (distance >= 0.f)
        {
            const std::pair<int, int> hitVertexFace {
                myTileMesh->GetVertexAndFace(Vec2 {rayStart + distance * rayDirection})};
            const int vertexIdx {hitVertexFace.first};
            const int faceIdx {hitVertexFace.second};

            if (faceIdx != -1)
            {
                myData.myHitVertex = vertexIdx;
                myData.myHitFace = faceIdx;
                return distance;
            }
        }
    }
    return -1.f;
}

const TileMeshCmp& TileMeshColliderCmp::GetTileMeshCmp() const
{
    const TileMeshCmp* cmp {GetEntity().GetComponent<TileMeshCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return *cmp;
}

const Transform& TileMeshColliderCmp::GetTransform() const
{
    const TransformCmp* cmp {GetEntity().GetComponent<TransformCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return cmp->GetTransform();
}
