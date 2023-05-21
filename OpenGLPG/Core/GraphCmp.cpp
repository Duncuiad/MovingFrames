#include "OpenGLPG_Base.h"

#include "GraphCmp.h"

#include "Assert.h"

GraphCmp::GraphCmp(const Filepath& aModelAsset, const Filepath& aShaderAsset)
    : myModelAsset {aModelAsset}
    , myShaderAsset {aShaderAsset}
{}

void GraphCmp::OnLoad(const LoadParams& someParams)
{
    myModel = someParams.myClientLoader.GetMeshLoader().GetModel(myModelAsset);
    myShader = someParams.myClientLoader.GetShaderLoader().GetShader(myShaderAsset);
}

void GraphCmp::Draw(const DrawParams& someParams) const
{
    ASSERT(myModel, "Trying to draw a model that wasn't loaded! Asset: {}", myModelAsset.GetBuffer());
    ASSERT(myShader, "Trying to draw with a shader that wasn't loaded! Asset: {}", myShaderAsset.GetBuffer());

    // @todo: implement SetUniform
    // myShader->SetUniformMat4("ModelMatrix", someParams.myModelMatrix);
    // @improvement: try and see if I can batch the use of shaders and meshes to minimize reallocation
    myShader->Use();
    myModel->Draw();
}
