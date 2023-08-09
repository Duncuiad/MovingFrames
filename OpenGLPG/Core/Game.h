#pragma once

#include "ClientLoader.h"
#include "DebugImGui.h"
#include "GameInputData.h"
#include "MathDefines.h"
#include "Renderer.h"
#include "World.h"

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
        Vec2 myViewportTL;
        Vec2 myViewportBR;
    };

    explicit Game(const ConstructionParams& someparams);
    void Init();
    void Shutdown();
    virtual void Update(const UpdateParams& someParams);
    void ResetGameInputData();
    void AddGameInput(const char* anInputName, float aValue);

protected:
    Renderer myRenderManager;
    World myWorld;

private:
#if DEBUG_IMGUI
    void DebugGameInput() const;
#else
    void DebugGameInput() const {};
#endif

    GameInputData myInputData;
};
