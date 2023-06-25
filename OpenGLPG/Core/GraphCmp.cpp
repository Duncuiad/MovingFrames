#include "OpenGLPG_Base.h"

#include "GraphCmp.h"

#include "Assert.h"
#include "Serializer.h"

REGISTER_SUBTYPE(GraphCmp)

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

    // @improvement: try and see if I can batch the use of shaders and meshes to minimize reallocation
    myShader->SetUniformMat4("Model", someParams.myModelMatrix);
    myShader->SetUniformMat4("WorldToClip", someParams.myWorldToClipMatrix);
    myShader->Use();
    myModel->Draw();
}

void GraphCmp::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myModel", myModelAsset);
    aSerializer.Process("myShader", myShaderAsset);
}
