#include "OpenGLPG_Base.h"

#include "ClientLoader.h"

// @improvement: find a place for system paths
ClientLoader::ClientLoader()
    : myShaderLoader {std::make_unique<ShaderLoader>("../Data/Assets/Shaders/")}
    , myModelLoader {std::make_unique<ModelLoader>("../Data/Assets/Models/")}
{}

void ClientLoader::Register() {}

void ClientLoader::Unregister() {}

void ClientLoader::Load() {}

void ClientLoader::Unload() {}
