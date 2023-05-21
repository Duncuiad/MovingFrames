#pragma once

#include "ModelLoader.h"
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
    ModelLoader& GetMeshLoader() const { return *myModelLoader; }

private:
    std::unique_ptr<ShaderLoader> myShaderLoader;
    std::unique_ptr<ModelLoader> myModelLoader;
};
