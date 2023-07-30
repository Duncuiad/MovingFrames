#pragma once

#include "EditorWidget.h"
#include "FrameSplineEditor.h"

class FrameSplineEditorWidget : public EditorWidget, public FrameSplineEditor
{
public:
    virtual void Draw() override;

private:
    bool DrawNewKeyAtBeginning();
    bool DrawKeyFrame(int anIndex);
    bool DrawKeySeparator(int anIndex);
    bool DrawKeyHeader(int anIndex);
    bool DrawDeletePopup();
    bool DrawNewKeyAtEnd();

    int myKeyToDelete {-1};
};
