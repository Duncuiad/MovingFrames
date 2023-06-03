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
        const glm::mat4& myModelMatrix;
        const glm::mat4& myWorldToClipMatrix;
    };

    GraphCmp(const Filepath& aModelAsset, const Filepath& aShaderAsset);
    void OnLoad(const LoadParams& someParams) override;
    void Update() override {}
    void Draw(const DrawParams& someParams) const;

private:
    Model::Ptr myModel {nullptr};
    Shader::Ptr myShader {nullptr};
    Filepath myModelAsset;
    Filepath myShaderAsset;
};
