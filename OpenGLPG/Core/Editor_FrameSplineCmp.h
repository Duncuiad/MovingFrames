#pragma once

#include "Component.h"
#include "EditorImGui.h"

class FrameSplineCmp;
class GraphCmp;

class Editor_FrameSplineCmp : public Component
{
    DECLARE_SUBTYPE(Editor_FrameSplineCmp)

public:
    void Serialize(Serializer& /*aSerializer*/) override {}
    void Update() override;

#if EDITOR_IMGUI
    void EditorWidgetImGui() override;
#endif

private:
    FrameSplineCmp& GetFrameSplineCmp() const;
    GraphCmp& GetGraphCmp() const;

    bool myIsDirty {true};
};
