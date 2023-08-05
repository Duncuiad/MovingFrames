#include "OpenGLPG_Base.h"

#include "Editor_FrameSplineCmp.h"

#include "Assert.h"
#include "Entity.h"
#include "FrameSpline.h"
#include "FrameSplineCmp.h"
#include "FrameSplineGraphCmp.h"

#include <functional>

REGISTER_SUBTYPE(Editor_FrameSplineCmp)

void Editor_FrameSplineCmp::OnEnterWorld()
{
    myWidget.AttachSplineObject(&GetFrameSplineCmp().mySpline);
    myWidget.ConnectOnChangedCallback(std::bind(&Editor_FrameSplineCmp::OnChanged, this));
}

void Editor_FrameSplineCmp::OnChanged() const
{
    ASSERT(myWidget.mySampleCount >= 2, "Too few samples");
    const float sampleDistance {1.f / static_cast<float>(myWidget.mySampleCount - 1)};

    const FrameSpline& spline {GetFrameSplineCmp().mySpline};
    FrameSplineGraphCmp& graphCmp {GetFrameSplineGraphCmp()};

    Array<FrameSplineGraphCmp::Key> graphKeys;

    for (int i = 0; i < myWidget.mySampleCount; ++i)
    {
        const MovingFrame frame {spline.Interpolate(i * sampleDistance)};
        const Vec3 position {frame.GetPosition()};
        const Mat3 orientation {frame.GetOrientation()};
        graphKeys.EmplaceBack(position, orientation[0], orientation[1], -orientation[2],
                              frame.GetLinearVelocity(MovingFrame::Extrinsic),
                              frame.GetAngularVelocity(MovingFrame::Extrinsic));
    }

    // @improv: move the array
    graphCmp.SetKeys(graphKeys);
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
