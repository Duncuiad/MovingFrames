#include "OpenGLPG_Base.h"

#include "Game.h"

Game::Game(const ConstructionParams& someparams) : myRenderManager {{someparams.myLoader.GetShaderLoader()}}
{
}

void Game::Init()
{
    myRenderManager.Init({});
}

void Game::Shutdown()
{
    myRenderManager.Shutdown();
}

void Game::Update()
{
    myRenderManager.Render({});
}
