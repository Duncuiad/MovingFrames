#include "OpenGLPG_Base.h"

#include "Game.h"

Game::Game(const ConstructionParams& someparams)
    : myRenderManager {{someparams.myLoader.GetShaderLoader()}}
{}

void Game::Init()
{
    myRenderManager.Init({});
    myWorldModel.Init();
}

void Game::Shutdown()
{
    myWorldModel.Shutdown();
    myRenderManager.Shutdown();
}

void Game::Update()
{
    myWorldModel.Update();

    const WorldModel* worldToRender {myWorldModel.IsAvailable() ? &myWorldModel : nullptr};
    myRenderManager.Render({worldToRender});
}
