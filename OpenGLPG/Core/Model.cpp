#include "OpenGLPG_Base.h"

#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

Model::Model(const Filepath& anAsset)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(anAsset.GetBuffer(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    ProcessAssimpNode(scene->mRootNode, scene);
}

Model::~Model()
{
    for (Mesh& mesh : myMeshes)
    {
        mesh.DeleteBuffers();
    }
}

void Model::Draw() const
{
    for (const Mesh& mesh : myMeshes)
    {
        mesh.Draw();
    }
}

void Model::ProcessAssimpNode(aiNode* aNode, const aiScene* aScene)
{
    for (unsigned int i = 0; i < aNode->mNumMeshes; i++)
    {
        aiMesh* mesh = aScene->mMeshes[aNode->mMeshes[i]];
        ProcessAssimpMesh(mesh, aScene);
    }
    for (unsigned int i = 0; i < aNode->mNumChildren; i++)
    {
        ProcessAssimpNode(aNode->mChildren[i], aScene);
    }
}

void Model::ProcessAssimpMesh(aiMesh* aMesh, const aiScene* aScene)
{
    Vertex::List vertices;
    Vertex::IndexList indices;

    for (unsigned int i = 0; i < aMesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = aMesh->mVertices[i].x;
        vector.y = aMesh->mVertices[i].y;
        vector.z = aMesh->mVertices[i].z;
        vertex.Position = vector;

        vector.x = aMesh->mNormals[i].x;
        vector.y = aMesh->mNormals[i].y;
        vector.z = aMesh->mNormals[i].z;
        vertex.Normal = vector;

        if (aMesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = aMesh->mTextureCoords[0][i].x;
            vec.y = aMesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < aMesh->mNumFaces; i++)
    {
        aiFace face = aMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    myMeshes.emplace_back(vertices, indices);
}
