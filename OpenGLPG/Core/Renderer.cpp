#include "OpenGLPG_Base.h"

#include "Renderer.h"

#include "CameraData.h"
#include "Entity.h"
#include "GraphCmp.h"
#include "MathUtils.h"
#include "WorldModel.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Renderer::Renderer(const ConstructionParams& someParams)
    : myShaderLoader {someParams.myShaderLoader} // @remove
{
    SetClearColor();
}

void Renderer::Init(const InitParams& someParams) {}

void Renderer::Shutdown() {}

void Renderer::Render(const RenderParams& someParams)
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (someParams.myWorldModel)
    {
        glm::mat4 viewportAdjustment {glm::identity<glm::mat4>()};
        int viewportData[4];
        glGetIntegerv(GL_VIEWPORT, viewportData);
        if (viewportData[2] > 0)
        {
            viewportAdjustment[0][0] =
                (static_cast<float>(viewportData[3]) * 16.f) / (static_cast<float>(viewportData[2]) * 9.f);
        }

        // @todo: refactor model-view-projection matrix calculations
        const CameraData& activeCameraData {someParams.myWorldModel->GetActiveCameraData()};
        const glm::mat4 view {glm::affineInverse(activeCameraData.myCameraTransform)};
        const glm::mat4 projection {viewportAdjustment *
                                    Utils::Projection(activeCameraData.myFOV, activeCameraData.myAspectRatio,
                                                      activeCameraData.myNear, activeCameraData.myFar)};
        const glm::mat4 worldToClip {viewportAdjustment * Utils::WorldToClip(activeCameraData)};

        for (const auto& [uid, entity] : someParams.myWorldModel->GetEntities())
        {
            // @todo: retrieve transform from transform component

            Transform entityTransform {Mat4 {glm::identity<glm::mat4>()}};
            entity.GetComponent<GraphCmp>()->Draw(
                {entityTransform, view, view * entityTransform, projection, worldToClip});
        }
    }
}

void Renderer::SetClearColor()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}
