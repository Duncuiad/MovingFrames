#include "OpenGLPG_Base.h"

#include "FrameSplineEditor.h"

#include "Assert.h"

#include <glm/common.hpp>
#include <utility>

void FrameSplineEditor::AttachSplineObject(FrameSpline* aSpline)
{
    mySpline = aSpline;
}

void FrameSplineEditor::AddKeyFrameAtBeginning() const
{
    ASSERT(mySpline != nullptr, "Failed attaching spline to widget");
    const auto& keys {mySpline->myKeyFrames};
    const float lastT {keys.Count() == 1 ? 0.f : keys.GetFirst().myTiming};
    MovingFrame& newFrame {mySpline->AddKeyFrame(glm::clamp(lastT, 0.f, 1.f))};
    if (keys.Count())
    {
        newFrame = keys.GetFirst().myFrame;
    }
}

void FrameSplineEditor::AddKeyFrameAtEnd() const
{
    ASSERT(mySpline != nullptr, "Failed attaching spline to widget");
    const auto& keys {mySpline->myKeyFrames};
    const float lastT {keys.Count() == 0 ? 0.f : keys.Count() == 1 ? 1.f : keys.GetLast().myTiming};
    MovingFrame& newFrame {mySpline->AddKeyFrame(glm::clamp(lastT, 0.f, 1.f))};
    if (keys.Count())
    {
        newFrame = keys.GetLast().myFrame;
    }
}

void FrameSplineEditor::InsertKeyFrameAtIndex(int anIndex) const
{
    ASSERT(mySpline != nullptr, "Failed attaching spline to widget");
    const auto& keys {mySpline->myKeyFrames};
    ASSERT(0 < anIndex && anIndex < keys.Count(), "Can't insert new keyframe");
    const float newTiming {(keys[anIndex - 1].myTiming + keys[anIndex].myTiming) * 0.5f};
    const MovingFrame halfwayFrame {mySpline->Interpolate(newTiming)};
    MovingFrame& newFrame {mySpline->AddKeyFrame(newTiming)};
    newFrame = halfwayFrame;
}

void FrameSplineEditor::SwapKeyFramesAtIndex(int anIndex) const
{
    ASSERT(mySpline != nullptr, "Failed attaching spline to widget");
    auto& keys {mySpline->myKeyFrames};
    ASSERT(0 < anIndex && anIndex < keys.Count(), "Can't insert new keyframe");
    std::swap(keys[anIndex - 1].myFrame, keys[anIndex].myFrame);
}

Array<FrameSpline::KeyFrame>& FrameSplineEditor::GetKeyFrames() const
{
    ASSERT(mySpline != nullptr, "Failed attaching spline to widget");
    return mySpline->myKeyFrames;
}

FrameSpline::Interpolator FrameSplineEditor::GetInterpolator() const
{
    ASSERT(mySpline != nullptr, "Failed attaching spline to widget");
    return mySpline->myType;
}

void FrameSplineEditor::SetInterpolator(FrameSpline::Interpolator anInterpolator) const
{
    ASSERT(mySpline != nullptr, "Failed attaching spline to widget");
    mySpline->myType = anInterpolator;
}
