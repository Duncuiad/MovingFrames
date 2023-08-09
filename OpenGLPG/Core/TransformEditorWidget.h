#pragma once

#include "EditorWidget.h"
#include "MovingFrame.h"

class TransformEditorWidget : public EditorWidget
{
public:
    void Draw() override;

    MovingFrame myMovingFrame;
    float myScale {1.f};

private:
    bool myIsEditModeExtrinsic {true};
};
