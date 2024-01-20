#pragma once

#include "Filepath.h"
#include "GameInputData.h"
#include "Renderer.h"
#include "ScreenshotWidget.h"
#include "ShaderLoader.h"

class World;

class ScreenshotManager
{
public:
    struct ConstructionParams
    {
        Renderer& myRenderer;
        ShaderLoader& myShaderLoader;
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
    ShaderLoader& myShaderLoader;
    Filepath myLocalScreenshotFolder;

    ScreenshotWidget myWidget;
};
