#include "OpenGLPG_Base.h"

#include "FrameSpline.h"

#include "Assert.h"
#include "Serializer.h"

#include <algorithm>

MovingFrame FrameSpline::Interpolate(float aTiming) const
{
    ASSERT(0.f <= aTiming && aTiming <= 1.f, "Invalid Timing");

    if (!myKeyFrames.Count())
    {
        return {};
    }

    const auto& keyBefore {
        std::upper_bound(myKeyFrames.begin(), myKeyFrames.end(), aTiming,
                         [](float aTiming, const KeyFrame& aFrame) { return aTiming < aFrame.myTiming; })};
    const auto& keyAfter {
        std::lower_bound(myKeyFrames.begin(), myKeyFrames.end(), aTiming,
                         [](const KeyFrame& aFrame, float aTiming) { return aFrame.myTiming < aTiming; })};
    ASSERT(keyBefore <= keyAfter, "Invalid Order");
    ASSERT(static_cast<unsigned int>(keyAfter - keyBefore) <= 1u, "Invalid KeyFrame ordering");

    if (keyAfter <= myKeyFrames.begin())
    {
        return myKeyFrames.GetFirst().myFrame;
    }
    else if (keyBefore >= myKeyFrames.end())
    {
        return myKeyFrames.GetLast().myFrame;
    }
    else if (keyBefore->myTiming == keyAfter->myTiming)
    {
        return keyBefore->myFrame;
    }

    const float internalT {(aTiming - keyBefore->myTiming) / (keyAfter->myTiming - keyBefore->myTiming)};
    const InterpolateInternalParams params {keyBefore->myFrame, keyAfter->myFrame, internalT};

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

void FrameSpline::Serialize(Serializer& aSerializer)
{
    {
        int type = static_cast<int>(myType);
        aSerializer.Process("myType", type);
        myType = static_cast<Interpolator>(type);
    }
    aSerializer.Process("myKeyFrames", myKeyFrames);
}

MovingFrame FrameSpline::InterpolateLinearSmoothstep(const InterpolateInternalParams& someParams) const
{
    // @todo: implement method
    return MovingFrame();
}

MovingFrame FrameSpline::InterpolateCubicBezier(const InterpolateInternalParams& someParams) const
{
    // @todo: implement method
    return MovingFrame();
}

void FrameSpline::KeyFrame::Serialize(Serializer& aSerializer)
{
    aSerializer.Process("myTiming", myTiming);
    aSerializer.Process("myFrame", myFrame);
}
