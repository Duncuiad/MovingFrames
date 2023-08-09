#pragma once

#include "EditorWidget.h"
#include "MovingFrame.h"

class TransformEditorWidget : public EditorWidget
{
public:
    void Draw() override;

    MovingFrame myMovingFrame;

private:
    bool myIsEditModeExtrinsic {true};
};
