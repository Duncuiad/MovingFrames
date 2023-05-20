#pragma once

#include "Defines.h"
#include "Filepath.h"

#include <memory>

class Mesh
{
public:
    using Ptr = std::shared_ptr<const Mesh>;
    Mesh(const Filepath& aMeshAsset);
    void Draw() const;

private:
    void SetupMesh();

    unsigned int myVertexCount {0u};
    unsigned int myElementCount {0u};

    GLUID myVAO;
    GLUID myVBO;
    GLUID myEBO;
};
