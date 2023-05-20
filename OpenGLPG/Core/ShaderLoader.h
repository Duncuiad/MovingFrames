#pragma once

#include "Filepath.h"
#include "Shader.h"

#include <unordered_map>

class ShaderLoader
{
public:
    ShaderLoader(const Filepath& aShaderFolder);

    Shader::Ptr GetShader(const Filepath& aFilepath);

private:
    std::unordered_map<Filepath, Shader::Ptr> myShaders;
    Filepath myShaderFolder;
};
