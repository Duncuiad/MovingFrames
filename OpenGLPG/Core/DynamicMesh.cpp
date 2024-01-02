#include "OpenGLPG_Base.h"

#include "DynamicMesh.h"

DynamicMesh::DynamicMesh()
    : Mesh {}
{
    glGenVertexArrays(1, &myVAO);
    glGenBuffers(1, &myVBO);
    glGenBuffers(1, &myEBO);
}

DynamicMesh::DynamicMesh(const Vertex::List& someVertices, const Vertex::IndexList& someIndices)
    : DynamicMesh {}
{
    UpdateMesh(someVertices, someIndices);
}

DynamicMesh::DynamicMesh(Vertex::List&& someVertices, Vertex::IndexList&& someIndices)
    : DynamicMesh {}
{
    UpdateMesh(someVertices, someIndices);
}

void DynamicMesh::UpdateMesh(const Vertex::List& someVertices, const Vertex::IndexList& someIndices)
{
    myVertices = someVertices;
    myIndices = someIndices;
    UpdateBuffers();
}

void DynamicMesh::UpdateMesh(Vertex::List&& someVertices, Vertex::IndexList&& someIndices)
{
    myVertices = std::move(someVertices);
    myIndices = std::move(someIndices);
    UpdateBuffers();
}

void DynamicMesh::UpdateBuffers()
{
    if (myVertices.size() == 0 || myIndices.size() == 0)
    {
        return;
    }

    glBindVertexArray(myVAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, myVBO);
        glBufferData(GL_ARRAY_BUFFER, myVertices.size() * sizeof(Vertex), &myVertices[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, myIndices.size() * sizeof(Vertex::Index), &myIndices[0], GL_DYNAMIC_DRAW);
    }

    {
        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, myNormal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, myTexCoords));
    }

    glBindVertexArray(0);
}
