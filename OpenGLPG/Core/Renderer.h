#pragma once

#include "Defines.h"
#include "MathDefines.h"
#include "ShaderLoader.h"

class WorldModel;

class Renderer
{
public:
    struct ConstructionParams
    {
        ShaderLoader& myShaderLoader;
    };
    struct InitParams
    {};
    struct RenderParams
    {
        const WorldModel* myWorldModel {nullptr};
        Vec2 myViewportTL;
        Vec2 myViewportBR;
    };

    explicit Renderer(const ConstructionParams& someParams);
    void Init(const InitParams& someParams);
    void Shutdown();

    void Render(const RenderParams& someParams);

private:
    void SetClearColor();

    ShaderLoader& myShaderLoader;
    GLUID myShaderProgram;
};
