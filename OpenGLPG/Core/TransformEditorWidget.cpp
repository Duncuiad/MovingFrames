#include "OpenGLPG_Base.h"

#include "TransformEditorWidget.h"

#include "ImGuiWidgets.h"

#include <imgui.h>

void TransformEditorWidget::Draw()
{
    ImGui::Text("Coord Type");
    ImGui::SameLine();
    if (ImGui::RadioButton("Extrinsic", myIsEditModeExtrinsic))
    {
        myIsEditModeExtrinsic = true;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Intrinsic", !myIsEditModeExtrinsic))
    {
        myIsEditModeExtrinsic = false;
    }

    bool changed {false};
    const MovingFrame::Coord coord {myIsEditModeExtrinsic ? MovingFrame::Extrinsic : MovingFrame::Intrinsic};
    changed |= Widgets::OrientationWidget("Ori", coord, myMovingFrame);
    changed |= Widgets::PositionWidget("Pos", coord, myMovingFrame);

    if (changed)
    {
        myOnChanged();
    }
}
