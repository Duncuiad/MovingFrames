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
#include "TileMeshColliderCmp.h"
#include "TileVertex.h"

#include <functional>
#include <utility>
#include <vector>

namespace
{
void AddTriangle(bool anIsTypeA, const std::reference_wrapper<const TileVertex>* aDataBuffer,
                 Vertex::List& someVerticesOut, Vertex::IndexList& someIndicesOut)
{

    const unsigned int firstVertexIdx {static_cast<unsigned int>(someVerticesOut.size())};
    someIndicesOut.push_back(firstVertexIdx);
    someIndicesOut.push_back(firstVertexIdx + 1u);
    someIndicesOut.push_back(firstVertexIdx + 2u);

    Vertex vertex0 {Vec3 {aDataBuffer[0].get().GetPosition(), 0.f},
                    Vec3 {aDataBuffer[0].get().myData.myColor ? 1.f : 0.f, 0.f, 0.f},
                    anIsTypeA ? Vec2 {0.5f, 0.5f} : Vec2 {0.f, 0.f}};
    Vertex vertex1 {Vec3 {aDataBuffer[1].get().GetPosition(), 0.f},
                    Vec3 {0.f, aDataBuffer[1].get().myData.myColor ? 1.f : 0.f, 0.f},
                    anIsTypeA ? Vec2 {0.f, 0.5f} : Vec2 {0.5f, 0.f}};
    Vertex vertex2 {Vec3 {aDataBuffer[2].get().GetPosition(), 0.f},
                    Vec3 {0.f, 0.f, aDataBuffer[2].get().myData.myColor ? 1.f : 0.f},
                    anIsTypeA ? Vec2 {0.f, 0.f} : Vec2 {0.5f, 0.5f}};

    someVerticesOut.push_back(std::move(vertex0));
    someVerticesOut.push_back(std::move(vertex1));
    someVerticesOut.push_back(std::move(vertex2));
}

void AddSquare(const Vec2& aUVOffset, const std::reference_wrapper<const TileVertex>* aDataBuffer,
               Vertex::List& someVerticesOut, Vertex::IndexList& someIndicesOut)
{
    const unsigned int firstVertexIdx {static_cast<unsigned int>(someVerticesOut.size())};
    someIndicesOut.push_back(firstVertexIdx);
    someIndicesOut.push_back(firstVertexIdx + 1u);
    someIndicesOut.push_back(firstVertexIdx + 2u);
    someIndicesOut.push_back(firstVertexIdx + 2u);
    someIndicesOut.push_back(firstVertexIdx + 3u);
    someIndicesOut.push_back(firstVertexIdx);

    Vertex vertex0 {Vec3 {aDataBuffer[0].get().GetPosition(), 0.f},
                    Vec3 {aDataBuffer[0].get().myData.myColor ? 1.f : 0.f, 0.f, 0.f}, aUVOffset + Vec2 {0.f, 0.f}};
    Vertex vertex1 {Vec3 {aDataBuffer[1].get().GetPosition(), 0.f},
                    Vec3 {0.f, aDataBuffer[1].get().myData.myColor ? 1.f : 0.f, 0.f}, aUVOffset + Vec2 {0.5f, 0.f}};
    Vertex vertex2 {Vec3 {aDataBuffer[2].get().GetPosition(), 0.f},
                    Vec3 {0.f, 0.f, aDataBuffer[2].get().myData.myColor ? 1.f : 0.f}, aUVOffset + Vec2 {0.5, 0.5f}};
    Vertex vertex3 {Vec3 {aDataBuffer[3].get().GetPosition(), 0.f},
                    Vec3 {0.f, aDataBuffer[3].get().myData.myColor ? 1.f : 0.f, 0.f}, aUVOffset + Vec2 {0.f, 0.5f}};

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

void Editor_TileMeshCmp::Update()
{
    using ClickAction = TileMeshEditorWidget::ClickAction;
    using ActionMode = TileMeshEditorWidget::ActionMode;

    const TileMeshColliderCmp& collider {GetTileMeshColliderCmp()};
    if (collider.myData.myWasHit)
    {
        TileVertex::Data* vertexData {GetTileMeshCmp().myTileMesh.GetVertexData(collider.myData.myHitVertex)};
        TileFace::Data* faceData {GetTileMeshCmp().myTileMesh.GetFaceData(collider.myData.myHitFace)};
        ASSERT(vertexData != nullptr, "Invalid vertex index");
        ASSERT(faceData != nullptr, "Invalid face index");

        int* selectedResource {nullptr};
        bool* color {nullptr};
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
                myWidget.myActionMode == ActionMode::Vertices ? collider.myData.myHitVertex : collider.myData.myHitFace;

            switch (myWidget.myClickAction)
            {
            case ClickAction::Inspect: {
                break;
            }
            case ClickAction::FlipColor: {
                *color = !(*color);
                break;
            }
            case ClickAction::PaintBlack: {
                *color = true;
                break;
            }
            case ClickAction::PaintWhite: {
                *color = false;
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
    Vertex::List vertices;
    Vertex::IndexList indices;

    const TileMesh& tileMesh {GetTileMeshCmp().myTileMesh};
    const auto trianglesA {tileMesh.GetTriangles(myWidget.myHeightToDisplay, 1)};
    const auto trianglesB {tileMesh.GetTriangles(myWidget.myHeightToDisplay, 2)};
    const auto squaresA {tileMesh.GetSquares(myWidget.myHeightToDisplay, 1)};
    const auto squaresB {tileMesh.GetSquares(myWidget.myHeightToDisplay, 2)};
    const auto squaresC {tileMesh.GetSquares(myWidget.myHeightToDisplay, 4)};

    const auto triangleVertexCount {trianglesA.size() + trianglesB.size()};
    const auto squareVertexCount {squaresA.size() + squaresB.size() + squaresC.size()};
    vertices.reserve(triangleVertexCount + squareVertexCount);
    indices.reserve(triangleVertexCount + (squareVertexCount / 2) * 3);

    for (int i = 0; i < trianglesA.size() / 3; ++i)
    {
        AddTriangle(true, &trianglesA[3 * i], vertices, indices);
    }

    for (int i = 0; i < trianglesB.size() / 3; ++i)
    {
        AddTriangle(false, &trianglesB[3 * i], vertices, indices);
    }

    for (int i = 0; i < squaresA.size() / 4; ++i)
    {
        constexpr Vec2 uvOffsetA {0.5f, 0.f};
        AddSquare(uvOffsetA, &squaresA[4 * i], vertices, indices);
    }

    for (int i = 0; i < squaresB.size() / 4; ++i)
    {
        constexpr Vec2 uvOffsetB {0.5f, 0.5f};
        AddSquare(uvOffsetB, &squaresB[4 * i], vertices, indices);
    }

    for (int i = 0; i < squaresC.size() / 4; ++i)
    {
        constexpr Vec2 uvOffsetC {0.f, 0.5f};
        AddSquare(uvOffsetC, &squaresC[4 * i], vertices, indices);
    }

    DynamicMeshGraphCmp& graphCmp {GetDynamicMeshGraphCmp()};
    graphCmp.SetMesh(std::move(vertices), std::move(indices));
    graphCmp.GetShader().Use();
    graphCmp.GetShader().SetUniformInt("ShowGraphs", static_cast<int>(myWidget.myShowGraphs));
    graphCmp.GetShader().SetUniformInt("ShowEdges", static_cast<int>(myWidget.myShowEdges));
    graphCmp.GetShader().SetUniformInt("ShowBlocks", static_cast<int>(myWidget.myShowBlocks));
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

const TileMeshColliderCmp& Editor_TileMeshCmp::GetTileMeshColliderCmp() const
{
    const TileMeshColliderCmp* cmp {GetEntity().GetComponent<TileMeshColliderCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return *cmp;
}
