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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (someParams.myWorld)
    {
        Mat4 viewportAdjustment {glm::identity<glm::mat4>()};
        int viewportData[4];
        glGetIntegerv(GL_VIEWPORT, viewportData);
        if (viewportData[2] > 0)
        {
            viewportAdjustment[0][0] =
                (static_cast<float>(viewportData[3]) * 16.f) / (static_cast<float>(viewportData[2]) * 9.f);
        }

        // @todo: refactor model-view-projection matrix calculations
        const CameraData& activeCameraData {someParams.myWorld->GetActiveCameraData()};
        const Mat4 view {glm::affineInverse(activeCameraData.myCameraTransform)};
        const Mat4 projection {viewportAdjustment * Utils::Projection(activeCameraData.myFOV,
                                                                      activeCameraData.myAspectRatio,
                                                                      activeCameraData.myNear, activeCameraData.myFar)};
        const Mat4 worldToClip {viewportAdjustment * Utils::WorldToClip(activeCameraData)};

        for (const auto& [uid, entity] : someParams.myWorld->GetEntities())
        {
            Transform entityTransform {entity.GetComponent<TransformCmp>()->GetTransform()};
            entity.GetComponent<GraphCmp>()->Draw(
                {entityTransform, view, view * entityTransform, projection, worldToClip});
        }
    }
}

void Renderer::SetClearColor()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}
