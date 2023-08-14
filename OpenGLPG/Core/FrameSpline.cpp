#include "OpenGLPG_Base.h"

#include "FrameSpline.h"

#include "Assert.h"
#include "InterpolationUtils.h"
#include "MathUtils.h"
#include "Serializer.h"

#include <algorithm>

const Array<FrameSpline::KeyFrame>& FrameSpline::GetKeyFrames() const
{
    return myKeyFrames;
}

MovingFrame FrameSpline::Interpolate(float aTiming) const
{
    ASSERT(0.f <= aTiming && aTiming <= 1.f, "Invalid Timing");

    if (!myKeyFrames.Count())
    {
        return {};
    }

    const auto& keyBefore {
        std::upper_bound(myKeyFrames.rbegin(), myKeyFrames.rend(), aTiming,
                         [](float aTiming, const KeyFrame& aFrame) { return aFrame.myTiming <= aTiming; })};
    const auto& keyAfter {
        std::upper_bound(myKeyFrames.begin(), myKeyFrames.end(), aTiming,
                         [](float aTiming, const KeyFrame& aFrame) { return aTiming <= aFrame.myTiming; })};

    if (keyAfter <= myKeyFrames.begin())
    {
        return myKeyFrames.GetFirst().myFrame;
    }
    else if (keyBefore <= myKeyFrames.rbegin())
    {
        return myKeyFrames.GetLast().myFrame;
    }
    else if (keyBefore->myTiming == keyAfter->myTiming)
    {
        return keyBefore->myFrame;
    }

    const float internalT {aTiming - keyBefore->myTiming};
    const float internalDT {keyAfter->myTiming - keyBefore->myTiming};
    const InterpolateInternalParams params {keyBefore->myFrame, keyAfter->myFrame, internalT, internalDT};

    switch (myType)
    {
    case Interpolator::LinearSmoothstep:
        return InterpolateLinearSmoothstep(params);
    case Interpolator::CubicBezier:
        return InterpolateCubicBezier(params);
    default:
        ASSERT(false, "Invalid interpolator");
        break;
    }

    return {};
}

MovingFrame& FrameSpline::AddKeyFrame(float aTiming)
{
    ASSERT(0.f <= aTiming && aTiming <= 1.f, "Invalid Timing");
    const auto& keyAfter {
        std::lower_bound(myKeyFrames.begin(), myKeyFrames.end(), aTiming,
                         [](const KeyFrame& aFrame, float aTiming) { return aFrame.myTiming < aTiming; })};

    const auto& newFrame {myKeyFrames.Insert(keyAfter, {})};
    newFrame->myTiming = aTiming;
    return newFrame->myFrame;
}

DualQuat FrameSpline::ComputeTwistNumerically(float aTiming) const
{
    constexpr float delta {0.01f};
    const float x0 {glm::max(aTiming - delta, 0.f)};
    const float x1 {glm::min(aTiming + delta, 1.f)};
    ASSERT(x0 < x1, "Invalid delta in calculation");

    if (myKeyFrames.Count() == 0)
    {
        return DualQuat {};
    }
    if (x0 == 0.f)
    {
        return myKeyFrames.GetFirst().myFrame.GetTwist();
    }
    if (x1 == 1.f)
    {
        return myKeyFrames.GetLast().myFrame.GetTwist();
    }

    const DualQuat y0 {Interpolate(x0).GetPose()};
    const DualQuat y1 {Interpolate(x1).GetPose()};
    const DualQuat offset {-2.f * Log(glm::inverse(y1) * y0)};
    return Pure(offset) / (x1 - x0);
}

void FrameSpline::Serialize(Serializer& aSerializer)
{
    {
        int type = static_cast<int>(myType);
        aSerializer.Process("myType", type);
        myType = static_cast<Interpolator>(type);
    }
    aSerializer.Process("myKeyFrames", myKeyFrames);
}

bool FrameSpline::IsTwistInterpolationNumerical() const
{
    switch (myType)
    {
    default:
        return true;
    }
}

MovingFrame FrameSpline::InterpolateLinearSmoothstep(const InterpolateInternalParams& someParams) const
{
    MovingFrame start {someParams.myFrom};
    MovingFrame end {someParams.myTo};
    start.Move(someParams.myT);
    end.Move(someParams.myT - someParams.myDT);

    const float t {someParams.myT / someParams.myDT};
    const float smoothT {(3.f - 2.f * t) * t * t};

    return MovingFrame {Sclerp(start.GetPose(), end.GetPose(), smoothT)};
}

MovingFrame FrameSpline::InterpolateCubicBezier(const InterpolateInternalParams& someParams) const
{
    const MovingFrame& p0 {someParams.myFrom};
    const MovingFrame& p3 {someParams.myTo};
    MovingFrame p1 {p0};
    MovingFrame p2 {p3};
    p1.Move(someParams.myDT / 3.f);
    p2.Move(-someParams.myDT / 3.f);

    MovingFrame p01 {p0};
    MovingFrame p23 {p3};
    p01.Move(someParams.myT / 3.f);
    p23.Move((someParams.myT - someParams.myDT) / 3.f);

    const float t {someParams.myT / someParams.myDT};
    const DualQuat p12 {Sclerp(p1.GetPose(), p2.GetPose(), t)};
    const DualQuat p012 {Sclerp(p01.GetPose(), p12, t)};
    const DualQuat p123 {Sclerp(p12, p23.GetPose(), t)};

    return MovingFrame {Sclerp(p012, p123, t)};
}

void FrameSpline::KeyFrame::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myTiming", myTiming);
    aSerializer.Process("myFrame", myFrame);
}
