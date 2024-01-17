#pragma once

#include "Defines.h"
#include "MathDefines.h"
#include "RenderPass.h"
#include "ShaderLoader.h"

#include <string>
#include <unordered_map>

class World;

class Renderer
{
public:
    struct RenderParams
    {
        const World* myWorld {nullptr};
        const Vec2& myViewportTL;
        const Vec2& myViewportBR;
        const std::string& myRenderPassID = "";
    };

    Renderer();
    void Update();
    void Shutdown();

    void Render(const RenderParams& someParams);
    void CreateRenderPass(const std::string& aRenderPassID, int aMaxWidth, int aMaxHeight);
    unsigned char* GetRenderedTexture(const std::string& aRenderPassID);

private:
    void SetClearColor();
    bool Bind(RenderPass& aRenderPass, const Vec2& aViewportSize);
    void Unbind(RenderPass& aRenderPass);
    void DrawWorld(const World& aWorld, const Vec2& aViewportSize) const;

    using RenderPassMap = std::unordered_map<std::string, RenderPass>;
    RenderPassMap myRenderPasses;
};
