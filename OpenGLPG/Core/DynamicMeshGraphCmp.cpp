#include "OpenGLPG_Base.h"

#include "DynamicMeshGraphCmp.h"

#include "Serializer.h"

REGISTER_SUBTYPE(DynamicMeshGraphCmp)

DynamicMeshGraphCmp::DynamicMeshGraphCmp()
    : myMesh {}
{}

DynamicMeshGraphCmp::~DynamicMeshGraphCmp()
{
    myMesh.DeleteBuffers();
}

void DynamicMeshGraphCmp::OnLoad(const LoadParams& someParams)
{
    Base::OnLoad(someParams);
}

void DynamicMeshGraphCmp::Draw(const DrawParams& someParams) const
{
    Base::PrepareDraw(someParams);
    myMesh.Draw();
}

void DynamicMeshGraphCmp::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myShader", myShaderAsset);
}

void DynamicMeshGraphCmp::SetMesh(const Vertex::List& someVertices, const Vertex::IndexList& someIndices)
{
    myMesh.UpdateMesh(someVertices, someIndices);
}

void DynamicMeshGraphCmp::SetMesh(Vertex::List&& someVertices, Vertex::IndexList&& someIndices)
{
    myMesh.UpdateMesh(std::move(someVertices), std::move(someIndices));
}
