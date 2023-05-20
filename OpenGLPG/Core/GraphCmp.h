#pragma once

#include "Component.h"
#include "Filepath.h"
#include "Mesh.h"

#include <glm/matrix.hpp>

class GraphCmp : public Component
{
public:
    struct DrawParams
    {
        const glm::mat4& myModel;
    };

    void OnLoad(const LoadParams& someParams) override;
    void Update() override {}
    void Draw(const DrawParams& someParams);

private:
    Mesh::Ptr myMesh {nullptr};
    Shader::Ptr myShader {nullptr};
    Filepath myMeshAsset;
    Filepath myShaderAsset;
};
