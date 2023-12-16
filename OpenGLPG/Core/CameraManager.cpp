#include "OpenGLPG_Base.h"

#include "CameraManager.h"

#include "Assert.h"
#include "CameraUtils.h"
#include "DebugUtils.h"

#include <algorithm>

void CameraManager::Update(const UpdateParams& someParams)
{
    if (!myCameraContainer.empty())
    {
        Camera& activeCamera {myCameraContainer.back()};
        const CameraInputData inputData {GetInputData(someParams.myGameInputData)};

        for (const CameraBehavior::UniquePtr& behavior : activeCamera.myBehaviors)
        {
            behavior->Update({activeCamera.myData, inputData, someParams.myDeltaTime});
        }

        UpdateRuntimeData(someParams.myDeltaTime, activeCamera.myData);
        myCameraData = ComputeCameraData(activeCamera.myData);
    }

#if DEBUG_IMGUI
    DebugDraw();
#endif
}

void CameraManager::ActivateCamera(const CameraActivationRequest& aRequest)
{
    myCameraContainer.push_back(myCameraFactory.CreateCamera(aRequest.mySettings, aRequest.GetCameraUID()));

    // @todo: start blend in
}

void CameraManager::DeactivateCamera(const CameraDeactivationRequest& aRequest)
{
    // @todo: start blend out

    [[maybe_unused]] const auto endBeforeRemove {myCameraContainer.end()};
    [[maybe_unused]] const auto endAfterRemove {
        std::remove_if(myCameraContainer.begin(), myCameraContainer.end(),
                       [&aRequest](const Camera& aCamera) { return aCamera.myUID == aRequest.myCameraUID; })};
    ASSERT(!(endBeforeRemove == endAfterRemove), "Deactivating non-existent camera");
}

CameraInputData CameraManager::GetInputData(const GameInputData& someGameInputData)
{
    CameraInputData inputData {};

    const GameInputData::InputContainer& inputs {someGameInputData.myInputs};
    using namespace GameInput;

    const auto computeCameraInput = [&inputs](const char* aGamepadInputName, const char* aPositiveKeyboardInputName,
                                              const char* aNegativeKeyboardInputName) {
        const float totalInput {inputs.at(aGamepadInputName) + inputs.at(aPositiveKeyboardInputName) -
                                inputs.at(aNegativeKeyboardInputName)};
        return glm::clamp(totalInput, -1.f, 1.f);
    };

    inputData.myLookRL = computeCameraInput(theGamepadLookHorizontal, theKeyboardLookRight, theKeyboardLookLeft);
    inputData.myLookUD = computeCameraInput(theGamepadLookVertical, theKeyboardLookUp, theKeyboardLookDown);
    inputData.myMoveRL = computeCameraInput(theGamepadMoveLateral, theKeyboardMoveRight, theKeyboardMoveLeft);
    inputData.myMoveUD = computeCameraInput(theGamepadMoveVertical, theKeyboardMoveUp, theKeyboardMoveDown);
    inputData.myMoveFB = computeCameraInput(theGamepadMoveFrontal, theKeyboardMoveForward, theKeyboardMoveBackward);

    return inputData;
}

void CameraManager::UpdateRuntimeData(float aDeltaTime, CameraRuntimeData& outActiveCameraRuntimeData) const
{
    using namespace TwistFlags;
    CameraRuntimeData& data {outActiveCameraRuntimeData};
    const auto& desiredTwist {data.myDesiredTwist};

    MovingFrame& reference {data.myReferenceFrame};
    reference.ResetVelocities();
    reference.AddAngularVelocity(MovingFrame::Coord::Extrinsic,
                                 desiredTwist[Component::Rotational | Coordinate::Extrinsic | Frame::Reference]);
    reference.AddAngularVelocity(MovingFrame::Coord::Intrinsic,
                                 desiredTwist[Component::Rotational | Coordinate::Intrinsic | Frame::Reference]);
    reference.AddLinearVelocity(MovingFrame::Coord::Extrinsic,
                                desiredTwist[Component::Translational | Coordinate::Extrinsic | Frame::Reference]);
    reference.AddLinearVelocity(MovingFrame::Coord::Intrinsic,
                                desiredTwist[Component::Translational | Coordinate::Intrinsic | Frame::Reference]);

    MovingFrame& local {data.myLocalFrame};
    local.ResetVelocities();
    local.AddAngularVelocity(MovingFrame::Coord::Extrinsic,
                             desiredTwist[Component::Rotational | Coordinate::Extrinsic | Frame::Local]);
    local.AddAngularVelocity(MovingFrame::Coord::Intrinsic,
                             desiredTwist[Component::Rotational | Coordinate::Intrinsic | Frame::Local]);
    local.AddLinearVelocity(MovingFrame::Coord::Extrinsic,
                            desiredTwist[Component::Translational | Coordinate::Extrinsic | Frame::Local]);
    local.AddLinearVelocity(MovingFrame::Coord::Intrinsic,
                            desiredTwist[Component::Translational | Coordinate::Intrinsic | Frame::Local]);

    reference.Move(aDeltaTime);
    local.Move(aDeltaTime);

    for (int i = 0; i < 8; ++i)
    {
        data.myDesiredTwist[i] = Vec3 {0.f};
    }
}

#if DEBUG_IMGUI
void CameraManager::DebugDraw() const
{
    ImGui::Begin("Camera Manager");

    if (ImGui::TreeNodeEx("Cameras", ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (const Camera& camera : myCameraContainer)
        {
            ImGui::Text("%s", camera.myUID.GetString().data());
        }
        ImGui::TreePop();
    }

    if (!myCameraContainer.empty())
    {
        if (ImGui::TreeNodeEx("Active Camera", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const Camera& activeCamera {myCameraContainer.back()};
            const CameraRuntimeData& data {activeCamera.myData};
            if (ImGui::TreeNodeEx("Data", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Text("Aspect Ratio: %9.4f", data.myAspectRatio);
                ImGui::Text("FOV         : %9.4f", data.myFOV);
                ImGui::Text("Ortho Size  : %9.4f", data.myOrthoSize);
                ImGui::Text("Near Plane  : %9.4f", data.myNearPlane);
                ImGui::Text("Far Plane   : %9.4f", data.myFarPlane);

                ImGui::NewLine();
                ImGui::Indent();
                ImGui::DrawDualQuatHeader();
                ImGui::Unindent();

                ImGui::Separator();
                ImGui::Text("World:");
                ImGui::Draw(data.myReferenceFrame * data.myLocalFrame);
                ImGui::Separator();
                ImGui::Text("Reference:");
                ImGui::Draw(data.myReferenceFrame);
                ImGui::Separator();
                ImGui::Text("Local:");
                ImGui::Draw(data.myLocalFrame);

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }

    ImGui::End();
}
#endif
