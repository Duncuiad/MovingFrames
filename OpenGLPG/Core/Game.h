#pragma once

#include "ClientLoader.h"
#include "Renderer.h"
#include "WorldModel.h"

struct GLFWwindow;

class Game
{
public:
    struct ConstructionParams
    {
        ClientLoader& myLoader;
    };

    explicit Game(const ConstructionParams& someparams);
    void Init();
    void Shutdown();
    void Update();

private:
    Renderer myRenderManager;
    WorldModel myWorldModel;
};
