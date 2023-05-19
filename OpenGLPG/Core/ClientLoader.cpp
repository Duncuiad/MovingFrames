#include "OpenGLPG_Base.h"

#include "ClientLoader.h"

ClientLoader::ClientLoader() : myShaderLoader {std::make_unique<ShaderLoader>()}
{
}

void ClientLoader::Register()
{
}

void ClientLoader::Unregister()
{
}

void ClientLoader::Load()
{
}

void ClientLoader::Unload()
{
}
