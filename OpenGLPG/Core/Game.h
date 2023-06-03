#pragma once

#include "ClientLoader.h"
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
    Renderer myRenderManager;
    WorldModel myWorldModel;
    GameInputData myInputData;
};
