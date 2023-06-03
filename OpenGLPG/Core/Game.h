#pragma once

#include "ClientLoader.h"
#include "DebugImGui.h"
#include "GameInputData.h"
#include "Renderer.h"
#include "WorldModel.h"

struct GLFWwindow;

class Game
{
public:
    struct ConstructionParams
    {
        const ClientLoader& myLoader;
    };
    struct UpdateParams
    {
        float myDeltaTime;
    };

    explicit Game(const ConstructionParams& someparams);
    void Init();
    void Shutdown();
    void Update(const UpdateParams& someParams);
    void ResetGameInputData();
    void AddGameInput(const char* anInputName, float aValue);

private:
#if DEBUG_IMGUI
    void DebugGameInput() const;
#else
    void DebugGameInput() const {};
#endif

    Renderer myRenderManager;
    WorldModel myWorldModel;
    GameInputData myInputData;
};
