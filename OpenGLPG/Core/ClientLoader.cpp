#include "OpenGLPG_Base.h"

#include "ClientLoader.h"

// @improvement: find a place for system paths
ClientLoader::ClientLoader()
    : myShaderLoader {std::make_unique<ShaderLoader>("../Data/Assets/Shaders/")}
    , myMeshLoader {std::make_unique<MeshLoader>("../Data/Assets/Meshes/")}
{}

void ClientLoader::Register() {}

void ClientLoader::Unregister() {}

void ClientLoader::Load() {}

void ClientLoader::Unload() {}
