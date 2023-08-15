#include "OpenGLPG_Base.h"

#include "Editor_FrameSplineCmp.h"

#include "Assert.h"
#include "Entity.h"
#include "FrameSpline.h"
#include "FrameSplineCmp.h"
#include "FrameSplineGraphCmp.h"
#include "MathUtils.h"

#include <functional>

REGISTER_SUBTYPE(Editor_FrameSplineCmp)

namespace Local
{
void AddKey(const MovingFrame& aFrame, Array<FrameSplineGraphCmp::Key>& someKeysOut)
{
    const Vec3 position {aFrame.GetPosition()};
    const Mat3 orientation {aFrame.GetOrientation()};
    someKeysOut.EmplaceBack(position, orientation[0], orientation[1], -orientation[2],
                            aFrame.GetLinearVelocity(MovingFrame::Extrinsic),
                            aFrame.GetAngularVelocity(MovingFrame::Extrinsic));
}
} // namespace Local

void Editor_FrameSplineCmp::OnEnterWorld()
{
    myWidget.AttachSplineObject(&GetFrameSplineCmp().mySpline);
    myWidget.ConnectOnChangedCallback(std::bind(&Editor_FrameSplineCmp::OnChanged, this));
    const FrameSplineGraphCmp& graphCmp {GetFrameSplineGraphCmp()};
    myWidget.myKeyScale = graphCmp.GetKeyScale();
    myWidget.myTangentScale = graphCmp.GetTangentScale();
    OnChanged();
}

void Editor_FrameSplineCmp::OnStartDisplayWidget()
{
    GetFrameSplineGraphCmp().myShowControlKeys = true;
}

void Editor_FrameSplineCmp::OnEndDisplayWidget()
{
    GetFrameSplineGraphCmp().myShowControlKeys = false;
}

void Editor_FrameSplineCmp::OnChanged() const
{
    ASSERT(myWidget.mySampleCount >= 2, "Too few samples");
    const float sampleDistance {1.f / static_cast<float>(myWidget.mySampleCount - 1)};

    const FrameSpline& spline {GetFrameSplineCmp().mySpline};
    FrameSplineGraphCmp& graphCmp {GetFrameSplineGraphCmp()};

    {
        Array<FrameSplineGraphCmp::Key> graphKeys;
        for (int i = 0; i < myWidget.mySampleCount; ++i)
        {
            const float interpolationT {i * sampleDistance};
            MovingFrame frame {spline.Interpolate(interpolationT)};
            frame.SetTwist(spline.ComputeTwistNumerically(interpolationT));
            Local::AddKey(frame, graphKeys);
        }
        // @improv: move the array
        graphCmp.SetKeys(graphKeys);
    }

    {
        Array<FrameSplineGraphCmp::Key> graphControlKeys;
        for (const FrameSpline::KeyFrame& keyframe : spline.GetKeyFrames())
        {
            Local::AddKey(keyframe.myFrame, graphControlKeys);
        }
        graphCmp.SetControlKeys(graphControlKeys);
    }

    graphCmp.SetKeyScale(myWidget.myKeyScale);
    graphCmp.SetTangentScale(myWidget.myTangentScale);
}

FrameSplineCmp& Editor_FrameSplineCmp::GetFrameSplineCmp() const
{
    FrameSplineCmp* cmp {GetEntity().GetEditableComponent<FrameSplineCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return *cmp;
}

FrameSplineGraphCmp& Editor_FrameSplineCmp::GetFrameSplineGraphCmp() const
{
    FrameSplineGraphCmp* cmp {GetEntity().GetEditableComponent<FrameSplineGraphCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return *cmp;
}
