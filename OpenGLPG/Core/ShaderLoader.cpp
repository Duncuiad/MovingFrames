#include "OpenGLPG_Base.h"

#include "ShaderLoader.h"

#include "Assert.h"

#include <fstream>

ShaderLoader::ShaderLoader(const Filepath& aShaderFolder)
    : myShaderFolder {aShaderFolder}
{}

Shader::Ptr ShaderLoader::GetShader(const Filepath& aFilepath)
{
    if (myShaders.contains(aFilepath))
    {
        return myShaders.at(aFilepath);
    }
    ASSERT(aFilepath.HasExtension("shader"), "Trying to load wrong file format. {} is not a shader!",
           aFilepath.GetBuffer());

    Filepath fullPath {myShaderFolder};
    fullPath.Append(aFilepath.GetBuffer());

    std::ifstream shaderFile;
    shaderFile.open(fullPath.GetBuffer());
    ASSERT(shaderFile.is_open(), "Failed opening shader {}", fullPath.GetBuffer());

    char vertexPath[512];
    char fragmentPath[512];
    char geometryPath[512];
    shaderFile.getline(vertexPath, 512);
    shaderFile.getline(fragmentPath, 512);
    shaderFile.getline(geometryPath, 512);
    shaderFile.close();

    Filepath vertexShader {myShaderFolder};
    Filepath fragmentShader {myShaderFolder};
    Filepath geometryShader {myShaderFolder};
    vertexShader.Append(vertexPath);
    fragmentShader.Append(fragmentPath);
    geometryShader.Append(geometryPath);

    // ASSERT(shaderFile.good(), "Failed loading shader {}", fullPath.GetBuffer());

    const Filepath* maybeGeometryShader {strcmp(geometryPath, "") ? &geometryShader : nullptr};
    const auto& pair {myShaders.try_emplace(aFilepath, new Shader {vertexShader, fragmentShader, maybeGeometryShader})};
    myFloatHijacks.emplace(std::make_pair(aFilepath, HijackedUniforms {}));
    return pair.first->second;
}

std::unique_ptr<ShaderLoader::HijackToken> ShaderLoader::Hijack(const char* aUniformName,
                                                                const std::function<float(float)>& aNewValue)
{
    const std::string name {aUniformName};
    for (auto& [shaderName, hijackedUniforms] : myFloatHijacks)
    {
        Shader::Ptr shader {GetShader(shaderName)};
        float currentValue {0.f};
        if (shader->GetUniformFloat(aUniformName, currentValue))
        {
            auto& uniforms {hijackedUniforms.myUniforms};
            ASSERT(uniforms.find(name) != uniforms.end() || uniforms.empty(),
                   "This uniform has already been hijacked!");
            uniforms.emplace(std::make_pair(name, currentValue));
            shader->SetUniformFloat(aUniformName, aNewValue(currentValue));
        }
    }

    return std::make_unique<HijackToken>(name, *this);
}

void ShaderLoader::Restore(const char* aUniformName)
{
    const std::string name {aUniformName};
    for (auto& [shaderName, hijackedUniforms] : myFloatHijacks)
    {
        Shader::Ptr shader {GetShader(shaderName)};
        auto& uniforms {hijackedUniforms.myUniforms};
        if (const auto iterator {uniforms.find(name)}; iterator != uniforms.end())
        {
            shader->SetUniformFloat(aUniformName, iterator->second);
            uniforms.erase(iterator);
        }
    }
}

ShaderLoader::HijackToken::HijackToken(const std::string& aUniformName, ShaderLoader& aShaderLoader)
    : myUniformName {aUniformName}
    , myLoader {aShaderLoader}
{}

ShaderLoader::HijackToken::~HijackToken()
{
    myLoader.Restore(myUniformName.c_str());
}
