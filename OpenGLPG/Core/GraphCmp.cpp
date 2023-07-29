#include "OpenGLPG_Base.h"

#include "GraphCmp.h"

GraphCmp::GraphCmp(const Filepath& aShaderAsset)
    : myShaderAsset {aShaderAsset}
{}

void GraphCmp::OnLoad(const LoadParams& someParams)
{
    myShader = someParams.myClientLoader.GetShaderLoader().GetShader(myShaderAsset);
}
