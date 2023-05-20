#pragma once

#include "ShaderLoader.h"

#include <memory>

class ClientLoader
{
public:
    ClientLoader();
    void Register();
    void Unregister();
    void Load();
    void Unload();

    ShaderLoader& GetShaderLoader() { return *myShaderLoader; }

private:
    std::unique_ptr<ShaderLoader> myShaderLoader {nullptr};
};
