#pragma once

#include "Array.h"
#include "FrameSpline.h"

class FrameSplineEditor
{
public:
    void AttachSplineObject(FrameSpline* aSpline);

protected:
    void AddKeyFrameAtBeginning() const;
    void AddKeyFrameAtEnd() const;
    void InsertKeyFrameAtIndex(int anIndex) const;
    void SwapKeyFramesAtIndex(int anIndex) const;
    Array<FrameSpline::KeyFrame>& GetKeyFrames() const;
    FrameSpline::Interpolator GetInterpolator() const;
    void SetInterpolator(FrameSpline::Interpolator anInterpolator) const;

    FrameSpline* mySpline {nullptr};
};
