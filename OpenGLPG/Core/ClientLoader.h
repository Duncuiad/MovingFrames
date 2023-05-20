#pragma once

#include "MeshLoader.h"
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

    ShaderLoader& GetShaderLoader() const { return *myShaderLoader; }
    MeshLoader& GetMeshLoader() const { return *myMeshLoader; }

private:
    std::unique_ptr<ShaderLoader> myShaderLoader;
    std::unique_ptr<MeshLoader> myMeshLoader;
};
