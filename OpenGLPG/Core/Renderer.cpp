#include "OpenGLPG_Base.h"

#include "Renderer.h"

#include "CameraData.h"
#include "Entity.h"
#include "GraphCmp.h"
#include "MathDefines.h"
#include "MathUtils.h"
#include "TransformCmp.h"
#include "World.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Renderer::Renderer()
{
    SetClearColor();
}
void Renderer::Update()
{
    for (auto& [id, pass] : myRenderPasses)
    {
        pass.Clear();
    }
}

void Renderer::Shutdown()
{
    for (auto& [id, pass] : myRenderPasses)
    {
        pass.Shutdown();
    }
}

void Renderer::Render(const RenderParams& someParams)
{
    bool successfullyBindedBuffer {true};
    RenderPassMap::iterator renderToTexturePass {myRenderPasses.end()};
    if (!someParams.myRenderPassID.empty())
    {
        renderToTexturePass = myRenderPasses.find(someParams.myRenderPassID);
        successfullyBindedBuffer &= renderToTexturePass != myRenderPasses.end();
    }
    if (someParams.myWorld)
    {
        const Vec2 viewPortSize {someParams.myViewportBR - someParams.myViewportTL};

        if (renderToTexturePass != myRenderPasses.end())
        {
            successfullyBindedBuffer &= Bind(renderToTexturePass->second, viewPortSize);
        }

        if (!successfullyBindedBuffer)
        {
            return;
        }

        DrawWorld(*someParams.myWorld, viewPortSize);

        if (renderToTexturePass != myRenderPasses.end())
        {
            Unbind(renderToTexturePass->second);
        }
    }
}

void Renderer::CreateRenderPass(const std::string& aRenderPassID, int aMaxWidth, int aMaxHeight)
{
    myRenderPasses.emplace(std::make_pair(aRenderPassID, RenderPass {aMaxWidth, aMaxHeight}));
}

unsigned char* Renderer::GetRenderedTexture(const std::string& aRenderPassID)
{
    if (const auto renderToTexturePass {myRenderPasses.find(aRenderPassID)};
        renderToTexturePass != myRenderPasses.end())
    {
        return renderToTexturePass->second.GetPixelData();
    }
    return nullptr;
}

void Renderer::SetClearColor()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

bool Renderer::Bind(RenderPass& aRenderPass, const Vec2& aViewportSize)
{
    bool success {true};

    int currentWidth, currentHeight, desiredWidth, desiredHeight;
    aRenderPass.GetSize(currentWidth, currentHeight);
    desiredWidth = static_cast<int>(aViewportSize.x);
    desiredHeight = static_cast<int>(aViewportSize.y);
    if (desiredWidth != currentWidth || desiredHeight != currentHeight)
    {
        success &= aRenderPass.Resize(desiredWidth, desiredHeight);
    }

    if (success)
    {
        aRenderPass.Bind();
    }

    return success;
}

void Renderer::Unbind(RenderPass& aRenderPass)
{
    aRenderPass.Unbind();
}

void Renderer::DrawWorld(const World& aWorld, const Vec2& aViewportSize) const
{
    const CameraData& activeCameraData {aWorld.GetActiveCameraData()};
    Mat4 viewportAdjustment {glm::identity<glm::mat4>()};
    viewportAdjustment[0][0] = aViewportSize.y / aViewportSize.x * activeCameraData.myAspectRatio;

    const Mat4 view {glm::affineInverse(activeCameraData.myCameraTransform)};
    const Mat4 projection {viewportAdjustment * Utils::Projection(activeCameraData)};
    const Mat4 worldToClip {viewportAdjustment * Utils::WorldToClip(activeCameraData)};

    for (const auto& [uid, entity] : aWorld.GetEntities())
    {
        const Transform& entityTransform {entity.GetComponent<TransformCmp>()->GetTransform()};
        entity.GetComponent<GraphCmp>()->Draw({entityTransform, view, view * entityTransform, projection, worldToClip});
    }
}
