#pragma once

#include "Filepath.h"
#include "GameInputData.h"
#include "Renderer.h"

class World;

class ScreenshotManager
{
public:
    struct ConstructionParams
    {
        Renderer& myRenderer;
        const Filepath& myLocalFolder;
    };
    struct UpdateParams
    {
        const World* myWorld;
        const GameInputData& myGameInputData;
    };

    ScreenshotManager(const ConstructionParams& someParams);
    void Update(const UpdateParams& someParams);

private:
    Renderer& myRenderer;
    Filepath myLocalScreenshotFolder;

    int myWidth {3840};
    int myHeight {2160};
};
