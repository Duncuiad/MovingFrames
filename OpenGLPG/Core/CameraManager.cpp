#include "OpenGLPG_Base.h"

#include "CameraManager.h"

#include "Assert.h"
#include "CameraUtils.h"

#include <algorithm>
#include <imgui.h>

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

        UpdateRuntimeData(activeCamera.myData);
        myCameraData = ComputeCameraData(activeCamera.myData);
    }
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

void CameraManager::UpdateRuntimeData(CameraRuntimeData& anActiveCameraRuntimeData) const
{
    using namespace TwistFlags;
    CameraRuntimeData& data {anActiveCameraRuntimeData};
    data.myReferenceFrame.AddAngularVelocity(
        MovingFrame::Coord::Extrinsic,
        data.myDesiredTwist[Component::Rotational | Coordinate::Extrinsic | Frame::Reference]);
    data.myReferenceFrame.AddAngularVelocity(
        MovingFrame::Coord::Intrinsic,
        data.myDesiredTwist[Component::Rotational | Coordinate::Intrinsic | Frame::Reference]);
    data.myReferenceFrame.AddLinearVelocity(
        MovingFrame::Coord::Extrinsic,
        data.myDesiredTwist[Component::Translational | Coordinate::Extrinsic | Frame::Reference]);
    data.myReferenceFrame.AddLinearVelocity(
        MovingFrame::Coord::Intrinsic,
        data.myDesiredTwist[Component::Translational | Coordinate::Intrinsic | Frame::Reference]);

    data.myLocalFrame.AddAngularVelocity(
        MovingFrame::Coord::Extrinsic,
        data.myDesiredTwist[Component::Rotational | Coordinate::Extrinsic | Frame::Local]);
    data.myLocalFrame.AddAngularVelocity(
        MovingFrame::Coord::Intrinsic,
        data.myDesiredTwist[Component::Rotational | Coordinate::Intrinsic | Frame::Local]);
    data.myLocalFrame.AddLinearVelocity(
        MovingFrame::Coord::Extrinsic,
        data.myDesiredTwist[Component::Translational | Coordinate::Extrinsic | Frame::Local]);
    data.myLocalFrame.AddLinearVelocity(
        MovingFrame::Coord::Intrinsic,
        data.myDesiredTwist[Component::Translational | Coordinate::Intrinsic | Frame::Local]);
}
