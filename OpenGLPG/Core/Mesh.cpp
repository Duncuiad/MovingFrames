#include "OpenGLPG_Base.h"

#include "Mesh.h"

Mesh::Mesh(const Vertex::List& someVertices, const Vertex::IndexList& someIndices)
    : myVertices {someVertices}
    , myIndices {someIndices}
{
    glGenVertexArrays(1, &myVAO);
    glGenBuffers(1, &myVBO);
    glGenBuffers(1, &myEBO);

    glBindVertexArray(myVAO);
    {
        // @improvement: consider allowing specifying different DRAW qualifiers for each mesh (e.g. GL_STATIC_DRAW)
        glBindBuffer(GL_ARRAY_BUFFER, myVBO);
        glBufferData(GL_ARRAY_BUFFER, myVertices.size() * sizeof(Vertex), &myVertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, myIndices.size() * sizeof(Vertex::Index), &myIndices[0], GL_STATIC_DRAW);
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

        glBindVertexArray(0);
    }
}

void Mesh::DeleteBuffers()
{
    glDeleteBuffers(1, &myEBO);
    glDeleteBuffers(1, &myVBO);
    glDeleteVertexArrays(1, &myVAO);
}

// @todo: implement mesh binding and drawing
void Mesh::Draw() const
{
    glBindVertexArray(myVAO);

    if (myIsWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT, GL_FILL);
    }

    glDrawElements(GL_TRIANGLES, myIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
