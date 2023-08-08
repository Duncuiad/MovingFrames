#pragma once

#include "Array.h"
#include "MovingFrame.h"
#include "Serializable.h"

class FrameSpline : public Serializable
{
public:
    enum class Interpolator {
        LinearSmoothstep,
        CubicBezier
    };

    struct KeyFrame : public Serializable
    {
        float myTiming {0.f};
        MovingFrame myFrame {};

        void Serialize(Serializer& aSerializer) override;
    };

    const Array<KeyFrame>& GetKeyFrames() const;
    MovingFrame Interpolate(float aTiming) const;
    MovingFrame& AddKeyFrame(float aTiming);

    void Serialize(Serializer& aSerializer) override;

private:
    struct InterpolateInternalParams
    {
        const MovingFrame& myFrom;
        const MovingFrame& myTo;
        float myT;
        float myDT;
    };

    friend class FrameSplineEditor;

    MovingFrame InterpolateLinearSmoothstep(const InterpolateInternalParams& someParams) const;
    MovingFrame InterpolateCubicBezier(const InterpolateInternalParams& someParams) const;

    Interpolator myType {Interpolator::LinearSmoothstep};
    Array<KeyFrame> myKeyFrames;
};
