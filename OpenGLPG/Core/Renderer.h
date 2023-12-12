#pragma once

#include "Defines.h"
#include "MathDefines.h"
#include "ShaderLoader.h"

class World;

class Renderer
{
public:
    struct RenderParams
    {
        const World* myWorld {nullptr};
        const Vec2& myViewportTL;
        const Vec2& myViewportBR;
    };

    Renderer();
    void Shutdown();

    void Render(const RenderParams& someParams);

private:
    void SetClearColor();

    GLUID myShaderProgram;
};
