#pragma once

#include "Filepath.h"
#include "Mesh.h"

#include <memory>

struct aiMesh;
struct aiNode;
struct aiScene;

class Model
{
public:
    using Ptr = std::shared_ptr<const Model>;

    Model(const Filepath& anAsset);
    ~Model();
    void Draw() const;

private:
    void ProcessAssimpNode(aiNode* aNode, const aiScene* aScene);
    void ProcessAssimpMesh(aiMesh* aMesh, const aiScene* aScene);

    Mesh::List myMeshes;
};
