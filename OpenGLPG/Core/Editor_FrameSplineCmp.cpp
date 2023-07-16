#include "OpenGLPG_Base.h"

#include "Editor_FrameSplineCmp.h"

#include "Assert.h"
#include "Entity.h"
#include "FrameSplineCmp.h"
#include "GraphCmp.h"

REGISTER_SUBTYPE(Editor_FrameSplineCmp)

void Editor_FrameSplineCmp::Update()
{
    if (myIsDirty)
    {
        // @todo: update GraphCmp
        myIsDirty = false;
    }
}

#if EDITOR_IMGUI
void Editor_FrameSplineCmp::EditorWidgetImGui() {}
#endif

FrameSplineCmp& Editor_FrameSplineCmp::GetFrameSplineCmp() const
{
    FrameSplineCmp* cmp {GetEntity().GetEditableComponent<FrameSplineCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return *cmp;
}

GraphCmp& Editor_FrameSplineCmp::GetGraphCmp() const
{
    GraphCmp* cmp {GetEntity().GetEditableComponent<GraphCmp>()};
    ASSERT(cmp != nullptr, "Couldn't find required component");
    return *cmp;
}
