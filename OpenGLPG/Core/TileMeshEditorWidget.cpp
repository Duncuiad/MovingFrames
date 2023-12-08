#include "OpenGLPG_Base.h"

#include "TileMeshEditorWidget.h"

#include "Assert.h"
#include "ImGuiWidgets.h"

void TileMeshEditorWidget::AttachTileMeshObject(TileMesh& aTileMesh)
{
    myTileMesh = &aTileMesh;
    myHeightToDisplay = myTileMesh->GetMaxHeight();
}

void TileMeshEditorWidget::Draw()
{
    ASSERT(myTileMesh != nullptr, "Failed attaching tilemesh object");
    bool changed {false};

    changed |= ImGui::Checkbox("Show Graphs", &myShowGraphs);
    changed |= ImGui::Checkbox("Show Edges", &myShowEdges);
    changed |= ImGui::Checkbox("Show Blocks", &myShowBlocks);

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
    ImGui::Separator();

    ImGui::SliderFloat("##Random Color", &myVertexColorThreshold, 0., 1., "Randomize Vertices = %.3f",
                       ImGuiSliderFlags_AlwaysClamp);
    ImGui::SameLine();
    if (ImGui::Button("Run"))
    {
        myTileMesh->RandomizeVertexColors(myVertexColorThreshold);
        changed = true;
    }

    if (changed)
    {
        myOnChanged();
    }
}
