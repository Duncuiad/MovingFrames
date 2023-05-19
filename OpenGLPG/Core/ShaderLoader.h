#pragma once

#include "Defines.h"

#include <string>
#include <unordered_map>

class ShaderLoader
{
public:
    void OnLoad();

    // @note: change to const if loader is not implemented to be lazy
    const std::string& GetShader(const std::string& aFilePath);

private:
    std::unordered_map<std::string, std::string> myShaders;
};
