#include "OpenGLPG_Base.h"

#include "MeshLoader.h"

#include "Assert.h"

MeshLoader::MeshLoader(const Filepath& aMeshFolder)
    : myMeshFolder {aMeshFolder}
{}

Mesh::Ptr MeshLoader::GetMesh(const Filepath& aFilepath)
{
    if (myMeshes.contains(aFilepath))
    {
        return myMeshes.at(aFilepath);
    }
    Filepath fullPath {myMeshFolder};
    fullPath.Append(aFilepath.GetBuffer());

    const auto& pair {myMeshes.try_emplace(aFilepath, new Mesh {fullPath})};
    return pair.first->second;
}
