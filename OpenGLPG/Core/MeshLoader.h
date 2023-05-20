#pragma once

#include "Filepath.h"
#include "Mesh.h"

#include <unordered_map>

class MeshLoader
{
public:
    MeshLoader(const Filepath& aMeshFolder);

    Mesh::Ptr GetMesh(const Filepath& aFilepath);

private:
    std::unordered_map<Filepath, Mesh::Ptr> myMeshes;
    Filepath myMeshFolder;
};
