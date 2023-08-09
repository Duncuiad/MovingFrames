#include "OpenGLPG_Base.h"

#include "Game.h"

#include "Assert.h"

Game::Game(const ConstructionParams& someparams)
    : myWorld {{someparams.myLoader, "baselevel"}}
{}

void Game::Init()
{
    myWorld.Init();
}

void Game::Shutdown()
{
    myWorld.Shutdown();
    myRenderManager.Shutdown();
}

void Game::Update(const UpdateParams& someParams)
{
    myWorld.Update({myInputData, someParams.myDeltaTime});

    const World* worldToRender {myWorld.IsAvailable() ? &myWorld : nullptr};
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
