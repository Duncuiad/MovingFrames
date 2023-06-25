#pragma once

#include "Component.h"
#include "Filepath.h"
#include "Mesh.h"

#include <glm/matrix.hpp>

class GraphCmp : public Component
{
    DECLARE_SUBTYPE(GraphCmp)

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

    void Serialize(Serializer& aSerializer) override;

private:
    Model::Ptr myModel {nullptr};
    Shader::Ptr myShader {nullptr};
    Filepath myModelAsset;
    Filepath myShaderAsset;
};
