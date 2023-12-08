#include "OpenGLPG_Base.h"

#include "GraphCmp.h"

#include <glm/gtc/matrix_inverse.hpp>

GraphCmp::GraphCmp(const Filepath& aShaderAsset)
    : myShaderAsset {aShaderAsset}
{}

void GraphCmp::OnLoad(const LoadParams& someParams)
{
    myShader = someParams.myClientLoader.GetShaderLoader().GetShader(myShaderAsset);
}

void GraphCmp::PrepareDraw(const DrawParams& someParams) const
{
    myShader->Use();
    myShader->SetUniformMat4("Model", someParams.myModelMatrix);
    myShader->SetUniformMat4("View", someParams.myViewMatrix);
    myShader->SetUniformMat4("ModelView", someParams.myModelViewMatrix);
    myShader->SetUniformMat3("ModelViewIT", glm::inverseTranspose(glm::mat3(someParams.myModelViewMatrix)));
    myShader->SetUniformMat4("Projection", someParams.myProjectionMatrix);
    myShader->SetUniformMat4("WorldToClip", someParams.myWorldToClipMatrix);
}

const Shader& GraphCmp::GetShader() const
{
    return *myShader;
}
