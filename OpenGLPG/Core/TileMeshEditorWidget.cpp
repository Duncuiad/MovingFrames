#include "OpenGLPG_Base.h"

#include "TileMeshEditorWidget.h"

#include "Assert.h"
#include "ImGuiWidgets.h"

void TileMeshEditorWidget::AttachTileMeshObject(TileMesh& aTileMesh)
{
    myTileMesh = &aTileMesh;
}

void TileMeshEditorWidget::Draw()
{
    ASSERT(myTileMesh != nullptr, "Failed attaching tilemesh object");
    bool changed {false};

    bool isResetting {false};
    ImGui::Text("Reset");
    if (ImGui::Button("Triangle A"))
    {
        myTileMesh->Reset(TileType::TriangleA);
        isResetting = true;
    }
    if (ImGui::Button("Triangle B"))
    {
        myTileMesh->Reset(TileType::TriangleB);
        isResetting = true;
    }
    if (ImGui::Button("Square A"))
    {
        myTileMesh->Reset(TileType::SquareA);
        isResetting = true;
    }
    if (ImGui::Button("Square B"))
    {
        myTileMesh->Reset(TileType::SquareB);
        isResetting = true;
    }
    if (ImGui::Button("Square C"))
    {
        myTileMesh->Reset(TileType::SquareC);
        isResetting = true;
    }
    if (isResetting)
    {
        myHeightToDisplay = 0;
    }
    ImGui::Separator();
    changed |= isResetting;

    if (ImGui::Button("Subdivide"))
    {
        myTileMesh->SubdivideAllFaces();
        ++myHeightToDisplay;
        changed = true;
    }
    changed |= ImGui::SliderInt("##Display Height", &myHeightToDisplay, 0, myTileMesh->GetMaxHeight(),
                                "Display Height = %d", ImGuiSliderFlags_AlwaysClamp);

    if (changed)
    {
        myOnChanged();
    }
}
