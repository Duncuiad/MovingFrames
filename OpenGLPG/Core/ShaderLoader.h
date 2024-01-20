#pragma once

#include "Filepath.h"
#include "Shader.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class ShaderLoader
{
public:
    ShaderLoader(const Filepath& aShaderFolder);

    Shader::Ptr GetShader(const Filepath& aFilepath);

    class HijackToken
    {
    public:
        HijackToken(const std::string& aUniformName, ShaderLoader& aShaderLoader);
        ~HijackToken();

    private:
        const std::string myUniformName;
        ShaderLoader& myLoader;
    };

    [[nodiscard]] std::unique_ptr<HijackToken> Hijack(const char* aUniformName,
                                                      const std::function<float(float)>& aNewValue);

private:
    void Restore(const char* aUniformName);

    struct HijackedUniforms
    {
        std::unordered_map<std::string, float> myUniforms;
    };

    std::unordered_map<Filepath, Shader::Ptr> myShaders;
    std::unordered_map<Filepath, HijackedUniforms> myFloatHijacks;
    Filepath myShaderFolder;
};
