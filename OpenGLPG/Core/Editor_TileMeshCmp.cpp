#include "OpenGLPG_Base.h"

#include "Editor_TileMeshCmp.h"

#include "Array.h"
#include "Assert.h"
#include "DynamicMeshGraphCmp.h"
#include "Entity.h"
#include "MathDefines.h"
#include "Mesh.h"
#include "Serializer.h"
#include "TileMesh.h"
#include "TileMeshCmp.h"

#include <functional>
#include <utility>

namespace
{
void AddTriangle(bool anIsTypeA, const Vec2* aPositionBuffer, Vertex::List& someVerticesOut,
                 Vertex::IndexList& someIndicesOut)
{

    const unsigned int firstVertexIdx {static_cast<unsigned int>(someVerticesOut.size())};
    someIndicesOut.push_back(firstVertexIdx);
    someIndicesOut.push_back(firstVertexIdx + 1u);
    someIndicesOut.push_back(firstVertexIdx + 2u);

    Vertex vertex0 {Vec3 {aPositionBuffer[0], 0.f}, Vec3 {0.f, 0.f, 1.f},
                    anIsTypeA ? Vec2 {0.f, 0.f} : Vec2 {0.5f, 0.5f}};
    Vertex vertex1 {Vec3 {aPositionBuffer[1], 0.f}, Vec3 {0.f, 0.f, 1.f},
                    anIsTypeA ? Vec2 {0.5f, 0.f} : Vec2 {0.f, 0.5f}};
    Vertex vertex2 {Vec3 {aPositionBuffer[2], 0.f}, Vec3 {0.f, 0.f, 1.f},
                    anIsTypeA ? Vec2 {0.5f, 0.5f} : Vec2 {0.f, 0.f}};

    someVerticesOut.push_back(std::move(vertex0));
    someVerticesOut.push_back(std::move(vertex1));
    someVerticesOut.push_back(std::move(vertex2));
}

void AddSquare(const Vec2& aUVOffset, const Vec2* aPositionBuffer, Vertex::List& someVerticesOut,
               Vertex::IndexList& someIndicesOut)
{
    const unsigned int firstVertexIdx {static_cast<unsigned int>(someVerticesOut.size())};
    someIndicesOut.push_back(firstVertexIdx);
    someIndicesOut.push_back(firstVertexIdx + 1u);
    someIndicesOut.push_back(firstVertexIdx + 2u);
    someIndicesOut.push_back(firstVertexIdx + 2u);
    someIndicesOut.push_back(firstVertexIdx + 3u);
    someIndicesOut.push_back(firstVertexIdx);

    Vertex vertex0 {Vec3 {aPositionBuffer[0], 0.f}, Vec3 {0.f, 0.f, 1.f}, aUVOffset + Vec2 {0.f, 0.f}};
    Vertex vertex1 {Vec3 {aPositionBuffer[1], 0.f}, Vec3 {0.f, 0.f, 1.f}, aUVOffset + Vec2 {0.5f, 0.f}};
    Vertex vertex2 {Vec3 {aPositionBuffer[2], 0.f}, Vec3 {0.f, 0.f, 1.f}, aUVOffset + Vec2 {0.5, 0.5f}};
    Vertex vertex3 {Vec3 {aPositionBuffer[3], 0.f}, Vec3 {0.f, 0.f, 1.f}, aUVOffset + Vec2 {0.f, 0.5f}};

    someVerticesOut.push_back(std::move(vertex0));
    someVerticesOut.push_back(std::move(vertex1));
    someVerticesOut.push_back(std::move(vertex2));
    someVerticesOut.push_back(std::move(vertex3));
}
} // namespace

REGISTER_SUBTYPE(Editor_TileMeshCmp)

void Editor_TileMeshCmp::OnEnterWorld()
{
    myWidget.AttachTileMeshObject(GetTileMeshCmp().myTileMesh);
    myWidget.ConnectOnChangedCallback(std::bind(&Editor_TileMeshCmp::OnChanged, this));
    OnChanged();

    GetDynamicMeshGraphCmp().SetIsWireframe(false);
}

void Editor_TileMeshCmp::OnChanged() const
{
    Vertex::List vertices;
    Vertex::IndexList indices;

    const TileMesh& tileMesh {GetTileMeshCmp().myTileMesh};
    const Array<Vec2> trianglesA {tileMesh.GetTriangles(myWidget.myHeightToDisplay, 1)};
    const Array<Vec2> trianglesB {tileMesh.GetTriangles(myWidget.myHeightToDisplay, 2)};
    const Array<Vec2> squaresA {tileMesh.GetSquares(myWidget.myHeightToDisplay, 1)};
    const Array<Vec2> squaresB {tileMesh.GetSquares(myWidget.myHeightToDisplay, 2)};
    const Array<Vec2> squaresC {tileMesh.GetSquares(myWidget.myHeightToDisplay, 4)};

    const int triangleVertexCount {trianglesA.Count() + trianglesB.Count()};
    const int squareVertexCount {squaresA.Count() + squaresB.Count() + squaresC.Count()};
    vertices.reserve(triangleVertexCount + squareVertexCount);
    indices.reserve(triangleVertexCount + (squareVertexCount / 2) * 3);

    for (int i = 0; i < trianglesA.Count() / 3; ++i)
    {
        AddTriangle(true, &trianglesA[3 * i], vertices, indices);
    }

    for (int i = 0; i < trianglesB.Count() / 3; ++i)
    {
        AddTriangle(false, &trianglesB[3 * i], vertices, indices);
    }

    for (int i = 0; i < squaresA.Count() / 4; ++i)
    {
        constexpr Vec2 uvOffsetA {0.5f, 0.f};
        AddSquare(uvOffsetA, &squaresA[4 * i], vertices, indices);
    }

    for (int i = 0; i < squaresB.Count() / 4; ++i)
    {
        constexpr Vec2 uvOffsetB {0.5f, 0.5f};
        AddSquare(uvOffsetB, &squaresB[4 * i], vertices, indices);
    }

    for (int i = 0; i < squaresC.Count() / 4; ++i)
    {
        constexpr Vec2 uvOffsetC {0.f, 0.5f};
        AddSquare(uvOffsetC, &squaresC[4 * i], vertices, indices);
    }

    GetDynamicMeshGraphCmp().SetMesh(std::move(vertices), std::move(indices));
}

TileMeshCmp& Editor_TileMeshCmp::GetTileMeshCmp() const
{
    TileMeshCmp* cmp {GetEntity().GetEditableComponent<TileMeshCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return *cmp;
}

DynamicMeshGraphCmp& Editor_TileMeshCmp::GetDynamicMeshGraphCmp() const
{
    DynamicMeshGraphCmp* cmp {GetEntity().GetEditableComponent<DynamicMeshGraphCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return *cmp;
}
