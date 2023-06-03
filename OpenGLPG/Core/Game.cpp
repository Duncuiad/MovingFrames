#include "OpenGLPG_Base.h"

#include "Game.h"

#include "Assert.h"

Game::Game(const ConstructionParams& someparams)
    : myRenderManager {{someparams.myLoader.GetShaderLoader()}}
    , myWorldModel {{someparams.myLoader}}
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

void Game::Update(const UpdateParams& someParams)
{
    myWorldModel.Update({myInputData, someParams.myDeltaTime});

    const WorldModel* worldToRender {myWorldModel.IsAvailable() ? &myWorldModel : nullptr};
    myRenderManager.Render({worldToRender});
}

void Game::ResetGameInputData()
{
    myInputData.Reset();
}

void Game::AddGameInput(const char* anInputName, float aValue)
{
    ASSERT(myInputData.myInputs.contains(anInputName), "Updating unknown input! Input name: {}", anInputName);
    myInputData.myInputs[anInputName] += aValue;
}
