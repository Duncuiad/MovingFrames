#pragma once

#include "EditorWidget.h"
#include "FrameSplineEditor.h"

class FrameSplineEditorWidget : public EditorWidget, public FrameSplineEditor
{
public:
    virtual void Draw() override;

    int mySampleCount {30};

private:
    bool DrawNewKeyAtBeginning();
    bool DrawKeyFrame(int anIndex);
    bool DrawKeySeparator(int anIndex);
    bool DrawKeyHeader(int anIndex);
    bool DrawDeletePopup();
    bool DrawNewKeyAtEnd();

    int myKeyToDelete {-1};
};
