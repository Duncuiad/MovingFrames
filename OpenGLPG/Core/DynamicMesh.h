#pragma once

#include "Mesh.h"

class DynamicMesh : public Mesh
{
public:
    DynamicMesh();
    DynamicMesh(const Vertex::List& someVertices, const Vertex::IndexList& someIndices);
    DynamicMesh(Vertex::List&& someVertices, Vertex::IndexList&& someIndices);

    void UpdateMesh(const Vertex::List& someVertices, const Vertex::IndexList& someIndices);
    void UpdateMesh(Vertex::List&& someVertices, Vertex::IndexList&& someIndices);

private:
    void UpdateBuffers();
};
