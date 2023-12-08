#pragma once

#include "Component.h"
#include "Filepath.h"
#include "Shader.h"

#include <glm/matrix.hpp>

class GraphCmp : public Component
{
public:
    struct DrawParams
    {
        const glm::mat4& myModelMatrix;
        const glm::mat4& myViewMatrix;
        const glm::mat4& myModelViewMatrix;
        const glm::mat4& myProjectionMatrix;
        const glm::mat4& myWorldToClipMatrix;
    };

    GraphCmp() = default;
    GraphCmp(const Filepath& aShaderAsset);
    void OnLoad(const LoadParams& someParams) override;
    void PrepareDraw(const DrawParams& someParams) const;

    virtual void Draw(const DrawParams& someParams) const = 0;

    const Shader& GetShader() const;

protected:
    Shader::Ptr myShader {nullptr};
    Filepath myShaderAsset;
};
