#pragma once

#include "Defines.h"
#include "MathDefines.h"
#include "ShaderLoader.h"

class WorldModel;

class Renderer
{
public:
    struct RenderParams
    {
        const WorldModel* myWorldModel {nullptr};
        Vec2 myViewportTL;
        Vec2 myViewportBR;
    };

    Renderer();
    void Shutdown();

    void Render(const RenderParams& someParams);

private:
    void SetClearColor();

    GLUID myShaderProgram;
};
