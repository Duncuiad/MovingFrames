#pragma once

#include "Filepath.h"
#include "ModelLoader.h"
#include "ShaderLoader.h"

#include <memory>

class ClientLoader
{
public:
    struct ConstructionParams
    {
        const Filepath& myAssetFolder;
    };

    ClientLoader(const ConstructionParams& someParams);
    void Register();
    void Unregister();
    void Load();
    void Unload();

    ShaderLoader& GetShaderLoader() const { return *myShaderLoader; }
    ModelLoader& GetMeshLoader() const { return *myModelLoader; }

    const Filepath myAssetFolder;
    const Filepath myShaderFolder;
    const Filepath myModelFolder;
    const Filepath myWorldFolder;
    const Filepath myTemplateFolder;

private:
    std::unique_ptr<ShaderLoader> myShaderLoader;
    std::unique_ptr<ModelLoader> myModelLoader;
};
