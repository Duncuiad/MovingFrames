#pragma once

#include "Defines.h"
#include "ShaderLoader.h"

class Renderer
{
public:
    struct ConstructionParams
    {
        ShaderLoader& myShaderLoader;
    };
    struct InitParams
    {
    };
    struct RenderParams
    {
    };

    explicit Renderer(const ConstructionParams& someParams);
    void Init(const InitParams& someParams);
    void LinkShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    void Shutdown();

    void Render(const RenderParams& someParams);

private:
    void SetClearColor();

    ShaderLoader& myShaderLoader;
    GLUID myShaderProgram;
};
