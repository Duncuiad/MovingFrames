#include "OpenGLPG_Base.h"

#include "Mesh.h"

// @todo: implement mesh loading
Mesh::Mesh(const Filepath& aMeshAsset)
{
    SetupMesh();
}

// @todo: implement mesh binding and drawing
void Mesh::Draw() const {}

// @todo: implement buffer generation
void Mesh::SetupMesh() {}
