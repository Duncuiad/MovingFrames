#pragma once

#include "DynamicMesh.h"
#include "GraphCmp.h"

class DynamicMeshGraphCmp : public GraphCmp
{
    DECLARE_SUBTYPE(DynamicMeshGraphCmp)
    using Base = GraphCmp;

public:
    DynamicMeshGraphCmp();
    ~DynamicMeshGraphCmp() override;

    void OnLoad(const LoadParams& someParams) override;
    void Draw(const DrawParams& someParams) const override;
    void Serialize(Serializer& aSerializer) override;

    void SetIsWireframe(bool anIsWireframe);
    void SetMesh(const Vertex::List& someVertices, const Vertex::IndexList& someIndices);
    void SetMesh(Vertex::List&& someVertices, Vertex::IndexList&& someIndices);

private:
    DynamicMesh myMesh;
};
