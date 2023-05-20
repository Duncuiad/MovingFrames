#pragma once

#include "Defines.h"
#include "Filepath.h"
#include "Shader.h"

#include <unordered_map>

class ShaderLoader
{
public:
    void OnLoad(const Filepath& aShaderFolder);

    Shader::Ptr GetShader(const Filepath& aFilepath);

private:
    std::unordered_map<Filepath, Shader::Ptr> myShaders;
    Filepath myShaderFolder {""};
};
