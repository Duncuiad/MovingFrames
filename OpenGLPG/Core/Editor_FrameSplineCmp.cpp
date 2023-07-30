#include "OpenGLPG_Base.h"

#include "Editor_FrameSplineCmp.h"

#include "Assert.h"
#include "Entity.h"
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
    // @todo: implement method
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
