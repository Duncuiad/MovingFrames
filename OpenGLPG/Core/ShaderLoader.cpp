#include "OpenGLPG_Base.h"

#include "ShaderLoader.h"

#include "Assert.h"

#include <fstream>

void ShaderLoader::OnLoad(const Filepath& aShaderFolder)
{
    myShaderFolder = aShaderFolder;
}

Shader::Ptr ShaderLoader::GetShader(const Filepath& aFilepath)
{
    if (myShaders.contains(aFilepath))
    {
        return myShaders.at(aFilepath);
    }
    ASSERT(aFilepath.HasExtension("shader"), "Trying to load wrong file format. %s is not a shader!",
           aFilepath.GetBuffer());

    Filepath fullPath {myShaderFolder};
    fullPath.Append(aFilepath.GetBuffer());

    std::ifstream shaderFile;
    shaderFile.open(fullPath.GetBuffer());
    ASSERT(shaderFile.is_open(), "Failed opening shader %s", fullPath.GetBuffer());

    char vertexPath[512];
    char fragmentPath[512];
    shaderFile.getline(vertexPath, 512);
    shaderFile.getline(fragmentPath, 512);
    shaderFile.close();

    Filepath vertexShader {myShaderFolder};
    Filepath fragmentShader {myShaderFolder};
    vertexShader.Append(vertexPath);
    fragmentShader.Append(fragmentPath);

    // ASSERT(shaderFile.good(), "Failed loading shader %s", fullPath.GetBuffer());

    const auto& pair {myShaders.try_emplace(aFilepath, new Shader {vertexShader, fragmentShader})};
    return pair.first->second;
}
