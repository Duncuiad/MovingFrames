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

REGISTER_SUBTYPE(Editor_TileMeshCmp)

void Editor_TileMeshCmp::OnEnterWorld()
{
    myWidget.AttachTileMeshObject(GetTileMeshCmp().myTileMesh);
    myWidget.ConnectOnChangedCallback(std::bind(&Editor_TileMeshCmp::OnChanged, this));
    OnChanged();
}

void Editor_TileMeshCmp::OnChanged() const
{
    const TileMesh& tileMesh {GetTileMeshCmp().myTileMesh};
    const auto meshData {tileMesh.GetMesh(myWidget.myHeightToDisplay)};

    const Array<Vec2>& tileMeshVertices {meshData.first};
    const Array<unsigned int>& tileMeshIndices {meshData.second};

    Vertex::List vertices;
    Vertex::IndexList indices;

    vertices.reserve(tileMeshVertices.Count());
    indices.reserve(tileMeshIndices.Count());

    for (const Vec2& vertexPosition : tileMeshVertices)
    {
        Vertex vertex {Vec3 {vertexPosition, 0.f}, Vec3 {0.f, 0.f, 1.f}, vertexPosition};
        vertices.push_back(std::move(vertex));
    }
    for (const unsigned int index : tileMeshIndices)
    {
        indices.push_back(index);
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
