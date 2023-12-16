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
void Renderer::Shutdown() {}

void Renderer::Render(const RenderParams& someParams)
{
    if (someParams.myWorld)
    {
        const Vec2 viewPortSize {someParams.myViewportBR - someParams.myViewportTL};
        const CameraData& activeCameraData {someParams.myWorld->GetActiveCameraData()};
        Mat4 viewportAdjustment {glm::identity<glm::mat4>()};
        viewportAdjustment[0][0] = viewPortSize.y / viewPortSize.x * activeCameraData.myAspectRatio;

        const Mat4 view {glm::affineInverse(activeCameraData.myCameraTransform)};
        const Mat4 projection {viewportAdjustment * Utils::Projection(activeCameraData)};
        const Mat4 worldToClip {viewportAdjustment * Utils::WorldToClip(activeCameraData)};

        for (const auto& [uid, entity] : someParams.myWorld->GetEntities())
        {
            const Transform& entityTransform {entity.GetComponent<TransformCmp>()->GetTransform()};
            entity.GetComponent<GraphCmp>()->Draw(
                {entityTransform, view, view * entityTransform, projection, worldToClip});
        }
    }
}

void Renderer::SetClearColor()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}
