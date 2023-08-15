#pragma once

#include "EditorWidget.h"
#include "FrameSplineEditor.h"

class FrameSplineEditorWidget : public EditorWidget, public FrameSplineEditor
{
public:
    void Draw() override;

    int mySampleCount {30};
    float myKeyScale {0.1f};
    float myTangentScale {1.0f};

private:
    bool DrawNewKeyAtBeginning();
    bool DrawKeyFrame(int anIndex);
    bool DrawKeySeparator(int anIndex);
    bool DrawKeyHeader(int anIndex);
    bool DrawNewKeyAtEnd();

    bool DrawDeletePopup();
    bool DrawInsertKeyPopup();

    int myKeyToDelete {-1};
    bool myIsEditModeExtrinsic {true};
    float myMinNewKeyT;
    float myMaxNewKeyT;
    float myNewKeyT {-1.f};
};
