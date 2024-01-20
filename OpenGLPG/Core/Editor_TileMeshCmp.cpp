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
#include <variant>
#include <vector>

namespace
{
void AddFace(const TileFace& aFace, const TileMesh& aTileMesh, bool anIsDisplayingFaces, Vertex::List& someVerticesOut,
             Vertex::IndexList& someIndicesOut)
{
    const TileVertex::Map& tileMeshVertices {aTileMesh.GetVertices()};
    const Array<TileHalfEdge>& tileMeshEdges {aTileMesh.GetEdges()};

    const unsigned int firstVertexIdx {static_cast<unsigned int>(someVerticesOut.size())};
    someIndicesOut.push_back(firstVertexIdx);
    someIndicesOut.push_back(firstVertexIdx + 1u);
    someIndicesOut.push_back(firstVertexIdx + 2u);

    const TileHalfEdge& edge0 {tileMeshEdges[aFace.myEdge]};
    const TileHalfEdge& edge1 {tileMeshEdges[edge0.myNext]};
    const TileHalfEdge& edge2 {tileMeshEdges[edge1.myNext]};
    const TileVertex& v0 {*tileMeshVertices.Find(edge0.myVertex)};
    const TileVertex& v1 {*tileMeshVertices.Find(edge1.myVertex)};
    const TileVertex& v2 {*tileMeshVertices.Find(edge2.myVertex)};

    if (aFace.IsTriangle())
    {
        const bool isTypeA {aFace.myType == TileType::TriangleA};
        Vertex vertex0 {Vec3 {edge0.myVertex.Pos(), 0.f}, Vec3 {v0.myData.myColor, 0.f, 0.f},
                        isTypeA ? Vec2 {0.49998f, 0.49999f} : Vec2 {0.00002f, 0.00001f}};
        Vertex vertex1 {Vec3 {edge1.myVertex.Pos(), 0.f}, Vec3 {0.f, v1.myData.myColor, 0.f},
                        isTypeA ? Vec2 {0.00001f, 0.49999f} : Vec2 {0.49999f, 0.00001f}};
        Vertex vertex2 {Vec3 {edge2.myVertex.Pos(), 0.f}, Vec3 {0.f, 0.f, v2.myData.myColor},
                        isTypeA ? Vec2 {0.00001f, 0.00002f} : Vec2 {0.49999f, 0.49998f}};

        if (anIsDisplayingFaces)
        {
            const float faceColor {aFace.myData.myColor};
            vertex0.myNormal = {faceColor, 0.f, 0.f};
            vertex1.myNormal = {0.f, faceColor, 0.f};
            vertex2.myNormal = {0.f, 0.f, faceColor};
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
        const TileVertex& v3 {*tileMeshVertices.Find(edge3.myVertex)};

        const Vec2 uvOffset {aFace.myType == TileType::SquareA   ? Vec2 {0.50001f, 0.f}
                             : aFace.myType == TileType::SquareB ? Vec2 {0.50001f, 0.50001f}
                                                                 : Vec2 {0.f, 0.50001f}};
        Vertex vertex0 {Vec3 {edge0.myVertex.Pos(), 0.f}, Vec3 {v0.myData.myColor, 0.f, 0.f},
                        uvOffset + Vec2 {0.f, 0.f}};
        Vertex vertex1 {Vec3 {edge1.myVertex.Pos(), 0.f}, Vec3 {0.f, v1.myData.myColor, 0.f},
                        uvOffset + Vec2 {0.49999f, 0.f}};
        Vertex vertex2 {Vec3 {edge2.myVertex.Pos(), 0.f}, Vec3 {0.f, 0.f, v2.myData.myColor},
                        uvOffset + Vec2 {0.49999f, 0.49999f}};
        Vertex vertex3 {Vec3 {edge3.myVertex.Pos(), 0.f}, Vec3 {0.f, v3.myData.myColor, 0.f},
                        uvOffset + Vec2 {0.f, 0.49999f}};

        if (anIsDisplayingFaces)
        {
            const float faceColor {aFace.myData.myColor};
            vertex0.myNormal = {faceColor, 0.f, 0.f};
            vertex1.myNormal = {0.f, faceColor, 0.f};
            vertex2.myNormal = {0.f, 0.f, faceColor};
            vertex3.myNormal = {0.f, faceColor, 0.f};
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
    // @todo: refactor function

    using ClickAction = TileMeshEditorWidget::ClickAction;
    using ShowBlocks = TileMeshEditorWidget::ShowBlocks;

    const TileMeshColliderCmp& collider {GetTileMeshColliderCmp()};
    for (const TileMeshColliderCmp::Hit& hit : collider.myHits)
    {
        if (hit.myTag != CollisionTag::PickLeftClick && hit.myTag != CollisionTag::PickRightClick)
        {
            continue;
        }

        TileMesh& tileMesh {GetTileMeshCmp().myTileMesh};
        TileVertex::Data* vertexData {tileMesh.GetVertexData(hit.myData.myHitVertex)};
        TileFace::Data* faceData {tileMesh.GetFaceData(hit.myData.myHitFace)};
        ASSERT(vertexData != nullptr, "Invalid vertex index");
        ASSERT(faceData != nullptr, "Invalid face index");

        std::variant<int*, std::optional<Dodec>*, bool> selectedResource {false};
        float* color {nullptr};
        switch (myWidget.myShowBlocks)
        {
        case ShowBlocks::Vertices: {
            selectedResource = &myWidget.mySelectedVertex;
            color = &vertexData->myColor;
            break;
        }
        case ShowBlocks::Faces: {
            selectedResource = &myWidget.mySelectedFace;
            color = &faceData->myColor;
            break;
        }
        default:
            break;
        }

        if (!std::holds_alternative<bool>(selectedResource) && color != nullptr)
        {
            if (std::holds_alternative<std::optional<Dodec>*>(selectedResource))
            {
                *std::get<std::optional<Dodec>*>(selectedResource) = hit.myData.myHitVertex;
            }
            else if (std::holds_alternative<int*>(selectedResource))
            {
                *std::get<int*>(selectedResource) = hit.myData.myHitFace;
            }

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
            case ClickAction::Neighbours: {
                std::optional<float> desiredColor;
                if (hit.myTag == CollisionTag::PickLeftClick)
                {
                    desiredColor = 0.f;
                }
                else if (hit.myTag == CollisionTag::PickRightClick)
                {
                    desiredColor = 1.f;
                }
                if (desiredColor.has_value() && myWidget.myShowBlocks == ShowBlocks::Vertices)
                {
                    *color = *desiredColor;
                    for (const Dodec& neighbour :
                         tileMesh.GetNeighbouringVertices(hit.myData.myHitVertex, myWidget.myHeightToDisplay))
                    {
                        tileMesh.GetVertexData(neighbour)->myColor = *desiredColor;
                    }
                }
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
    graphCmp.GetShader().SetUniformFloat("SmoothSize", 3.f);
    GetTileMeshColliderCmp().myFaceHeight = myWidget.myHeightToDisplay;
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
