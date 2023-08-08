#pragma once

#include "EditorCmp.h"
#include "FrameSplineEditorWidget.h"

class FrameSplineCmp;
class FrameSplineGraphCmp;

class Editor_FrameSplineCmp : public EditorCmp<FrameSplineEditorWidget>
{
    DECLARE_SUBTYPE(Editor_FrameSplineCmp)

public:
    void Serialize(Serializer& /*aSerializer*/) override {}
    void OnEnterWorld() override;
    void OnStartDisplayWidget() override;
    void OnEndDisplayWidget() override;
    void OnChanged() const;

private:
    FrameSplineCmp& GetFrameSplineCmp() const;
    FrameSplineGraphCmp& GetFrameSplineGraphCmp() const;
};
