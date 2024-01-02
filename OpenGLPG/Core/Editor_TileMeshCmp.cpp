#include "OpenGLPG_Base.h"

#include "Editor_TileMeshCmp.h"

#include "Array.h"
#include "Assert.h"
#include "DynamicMeshGraphCmp.h"
#include "Entity.h"
#include "MathDefines.h"
#include "Mesh.h"
#include "Serializer.h"
#include "TileHalfEdge.h"
#include "TileMesh.h"
#include "TileMeshCmp.h"
#include "TileMeshColliderCmp.h"
#include "TileVertex.h"

#include <functional>
#include <utility>
#include <vector>

namespace
{
void AddFace(const TileFace& aFace, const TileMesh& aTileMesh, bool anIsDisplayingFaces, Vertex::List& someVerticesOut,
             Vertex::IndexList& someIndicesOut)
{
    const Array<TileVertex>& tileMeshVertices {aTileMesh.GetVertices()};
    const Array<TileHalfEdge>& tileMeshEdges {aTileMesh.GetEdges()};

    const unsigned int firstVertexIdx {static_cast<unsigned int>(someVerticesOut.size())};
    someIndicesOut.push_back(firstVertexIdx);
    someIndicesOut.push_back(firstVertexIdx + 1u);
    someIndicesOut.push_back(firstVertexIdx + 2u);

    const TileHalfEdge& edge0 {tileMeshEdges[aFace.myEdge]};
    const TileHalfEdge& edge1 {tileMeshEdges[edge0.myNext]};
    const TileHalfEdge& edge2 {tileMeshEdges[edge1.myNext]};
    const TileVertex& v0 {tileMeshVertices[edge0.myVertex]};
    const TileVertex& v1 {tileMeshVertices[edge1.myVertex]};
    const TileVertex& v2 {tileMeshVertices[edge2.myVertex]};

    if (aFace.IsTriangle())
    {
        const bool isTypeA {aFace.myType == TileType::TriangleA};
        Vertex vertex0 {Vec3 {v0.GetPosition(), 0.f}, Vec3 {v0.myData.myColor, 0.f, 0.f},
                        isTypeA ? Vec2 {0.5f, 0.5f} : Vec2 {0.f, 0.f}};
        Vertex vertex1 {Vec3 {v1.GetPosition(), 0.f}, Vec3 {0.f, v1.myData.myColor, 0.f},
                        isTypeA ? Vec2 {0.f, 0.5f} : Vec2 {0.5f, 0.f}};
        Vertex vertex2 {Vec3 {v2.GetPosition(), 0.f}, Vec3 {0.f, 0.f, v2.myData.myColor},
                        isTypeA ? Vec2 {0.f, 0.f} : Vec2 {0.5f, 0.5f}};

        if (anIsDisplayingFaces)
        {
            const float faceColor {aFace.myData.myColor};
            vertex0.myNormal = {faceColor, faceColor, faceColor};
            vertex1.myNormal = {faceColor, faceColor, faceColor};
            vertex2.myNormal = {faceColor, faceColor, faceColor};
        }

        someVerticesOut.push_back(std::move(vertex0));
        someVerticesOut.push_back(std::move(vertex1));
        someVerticesOut.push_back(std::move(vertex2));
    }
    else
    {
        someIndicesOut.push_back(firstVertexIdx + 2u);
        someIndicesOut.push_back(firstVertexIdx + 3u);
        someIndicesOut.push_back(firstVertexIdx);

        const TileHalfEdge& edge3 {tileMeshEdges[edge2.myNext]};
        const TileVertex& v3 {tileMeshVertices[edge3.myVertex]};

        const Vec2 uvOffset {aFace.myType == TileType::SquareA   ? Vec2 {0.5f, 0.f}
                             : aFace.myType == TileType::SquareB ? Vec2 {0.5f, 0.5f}
                                                                 : Vec2 {0.f, 0.5f}};
        Vertex vertex0 {Vec3 {v0.GetPosition(), 0.f}, Vec3 {v0.myData.myColor, 0.f, 0.f}, uvOffset + Vec2 {0.f, 0.f}};
        Vertex vertex1 {Vec3 {v1.GetPosition(), 0.f}, Vec3 {0.f, v1.myData.myColor, 0.f}, uvOffset + Vec2 {0.5f, 0.f}};
        Vertex vertex2 {Vec3 {v2.GetPosition(), 0.f}, Vec3 {0.f, 0.f, v2.myData.myColor}, uvOffset + Vec2 {0.5, 0.5f}};
        Vertex vertex3 {Vec3 {v3.GetPosition(), 0.f}, Vec3 {0.f, v3.myData.myColor, 0.f}, uvOffset + Vec2 {0.f, 0.5f}};

        if (anIsDisplayingFaces)
        {
            const float faceColor {aFace.myData.myColor};
            vertex0.myNormal = {faceColor, faceColor, faceColor};
            vertex1.myNormal = {faceColor, faceColor, faceColor};
            vertex2.myNormal = {faceColor, faceColor, faceColor};
            vertex3.myNormal = {faceColor, faceColor, faceColor};
        }
        someVerticesOut.push_back(std::move(vertex0));
        someVerticesOut.push_back(std::move(vertex1));
        someVerticesOut.push_back(std::move(vertex2));
        someVerticesOut.push_back(std::move(vertex3));
    }
}

void UpdateMesh(const int aHeightToDisplay, bool anIsDisplayingFaces, const TileMeshCmp& aTileMeshCmp,
                DynamicMeshGraphCmp& aGraphCmpOut)
{
    Vertex::List vertices;
    Vertex::IndexList indices;

    const TileMesh& tileMesh {aTileMeshCmp.myTileMesh};

    for (const TileFace& face : tileMesh.GetFaces())
    {
        if (face.myHeight == aHeightToDisplay)
        {
            AddFace(face, tileMesh, anIsDisplayingFaces, vertices, indices);
        }
    }

    aGraphCmpOut.SetMesh(std::move(vertices), std::move(indices));
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

void Editor_TileMeshCmp::Update()
{
    using ClickAction = TileMeshEditorWidget::ClickAction;
    using ActionMode = TileMeshEditorWidget::ActionMode;

    const TileMeshColliderCmp& collider {GetTileMeshColliderCmp()};
    for (const TileMeshColliderCmp::Hit& hit : collider.myHits)
    {
        if (hit.myTag != CollisionTag::PickLeftClick && hit.myTag != CollisionTag::PickRightClick)
        {
            continue;
        }

        TileVertex::Data* vertexData {GetTileMeshCmp().myTileMesh.GetVertexData(hit.myData.myHitVertex)};
        TileFace::Data* faceData {GetTileMeshCmp().myTileMesh.GetFaceData(hit.myData.myHitFace)};
        ASSERT(vertexData != nullptr, "Invalid vertex index");
        ASSERT(faceData != nullptr, "Invalid face index");

        int* selectedResource {nullptr};
        float* color {nullptr};
        switch (myWidget.myActionMode)
        {
        case ActionMode::Vertices: {
            selectedResource = &myWidget.mySelectedVertex;
            color = &vertexData->myColor;
            break;
        }
        case ActionMode::Faces: {
            selectedResource = &myWidget.mySelectedFace;
            color = &faceData->myColor;
            break;
        }
        default:
            break;
        }

        if (selectedResource != nullptr && color != nullptr)
        {
            *selectedResource =
                myWidget.myActionMode == ActionMode::Vertices ? hit.myData.myHitVertex : hit.myData.myHitFace;

            switch (myWidget.myClickAction)
            {
            case ClickAction::Inspect: {
                break;
            }
            case ClickAction::Paint: {
                *color = (hit.myTag == CollisionTag::PickLeftClick)
                             ? 0.f
                             : ((hit.myTag == CollisionTag::PickRightClick) ? 1.f : *color);
                break;
            }
            default: {
                break;
            }
            }
        }
        OnChanged();
        collider.Reset();
    }
}

void Editor_TileMeshCmp::OnChanged() const
{
    DynamicMeshGraphCmp& graphCmp {GetDynamicMeshGraphCmp()};
    const bool isDisplayingFaces {myWidget.myShowBlocks == TileMeshEditorWidget::ShowBlocks::Faces};
    UpdateMesh(myWidget.myHeightToDisplay, isDisplayingFaces, GetTileMeshCmp(), graphCmp);
    graphCmp.GetShader().Use();
    graphCmp.GetShader().SetUniformInt("ShowGraphs", static_cast<int>(myWidget.myShowGraphs));
    graphCmp.GetShader().SetUniformInt("ShowEdges", static_cast<int>(myWidget.myShowEdges));
    graphCmp.GetShader().SetUniformInt("ShowBlocks", static_cast<int>(myWidget.myShowBlocks));
    GetTileMeshColliderCmp().myMaxFaceHeight = myWidget.myHeightToDisplay;
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

TileMeshColliderCmp& Editor_TileMeshCmp::GetTileMeshColliderCmp() const
{
    TileMeshColliderCmp* cmp {GetEntity().GetEditableComponent<TileMeshColliderCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return *cmp;
}
