#include "OpenGLPG_Base.h"

#include "Game.h"

#include "Assert.h"

Game::Game(const ConstructionParams& someparams)
    : myWorldModel {{someparams.myLoader, "baselevel"}}
{}

void Game::Init()
{
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

    DebugGameInput();
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

#if DEBUG_IMGUI
void Game::DebugGameInput() const
{
    ImGui::Begin("Game Input");
    for (const auto& [name, value] : myInputData.myInputs)
    {
        ImGui::Text("%s: %7.3f", name.data(), value);
    }
    ImGui::End();
}
#endif
