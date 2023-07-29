#include "OpenGLPG_Base.h"

#include "ModelGraphCmp.h"

#include "Assert.h"
#include "Serializer.h"

REGISTER_SUBTYPE(ModelGraphCmp)

ModelGraphCmp::ModelGraphCmp(const Filepath& aModelAsset, const Filepath& aShaderAsset)
    : Base {aShaderAsset}
    , myModelAsset {aModelAsset}
{}

void ModelGraphCmp::OnLoad(const LoadParams& someParams)
{
    Base::OnLoad(someParams);
    myModel = someParams.myClientLoader.GetMeshLoader().GetModel(myModelAsset);
}

void ModelGraphCmp::Draw(const DrawParams& someParams) const
{
    ASSERT(myModel, "Trying to draw a model that wasn't loaded! Asset: {}", myModelAsset.GetBuffer());
    ASSERT(myShader, "Trying to draw with a shader that wasn't loaded! Asset: {}", myShaderAsset.GetBuffer());

    // @improvement: try and see if I can batch the use of shaders and meshes to minimize reallocation
    myShader->SetUniformMat4("Model", someParams.myModelMatrix);
    myShader->SetUniformMat4("View", someParams.myViewMatrix);
    myShader->SetUniformMat4("ModelView", someParams.myModelViewMatrix);
    myShader->SetUniformMat4("Projection", someParams.myProjectionMatrix);
    myShader->SetUniformMat4("WorldToClip", someParams.myWorldToClipMatrix);
    myShader->Use();
    myModel->Draw();
}

void ModelGraphCmp::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myModel", myModelAsset);
    aSerializer.Process("myShader", myShaderAsset);
}
