#include "OpenGLPG_Base.h"

#include "ClientLoader.h"

ClientLoader::ClientLoader(const ConstructionParams& someParams)
    : myAssetFolder {someParams.myAssetFolder}
    , myShaderFolder {someParams.myAssetFolder + "Shaders/"}
    , myModelFolder {someParams.myAssetFolder + "Models/"}
    , myWorldFolder {someParams.myAssetFolder + "Worlds/"}
    , myTemplateFolder {someParams.myAssetFolder + "Templates/"}
    , myShaderLoader {std::make_unique<ShaderLoader>(myShaderFolder)}
    , myModelLoader {std::make_unique<ModelLoader>(myModelFolder)}
{}

void ClientLoader::Register() {}

void ClientLoader::Unregister() {}

void ClientLoader::Load() {}

void ClientLoader::Unload() {}
