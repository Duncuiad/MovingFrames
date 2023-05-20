#include "OpenGLPG_Base.h"

#include "GraphCmp.h"

#include "Assert.h"

void GraphCmp::OnLoad(const LoadParams& someParams)
{
    myMesh = someParams.myClientLoader.GetMeshLoader().GetMesh(myMeshAsset);
    myShader = someParams.myClientLoader.GetShaderLoader().GetShader(myShaderAsset);
}

void GraphCmp::Draw(const DrawParams& someParams)
{
    ASSERT(myMesh, "Trying to draw a mesh that wasn't loaded! Asset: {}", myMeshAsset.GetBuffer());
    ASSERT(myShader, "Trying to draw with a shader that wasn't loaded! Asset: {}", myShaderAsset.GetBuffer());

    // @todo: implement SetUniform
    // myShader->SetUniformMat4("Model", someParams.myModel);
    // @improvement: try and see if I can batch the use of shaders and meshes to minimize reallocation
    myShader->Use();
    myMesh->Draw();
}
