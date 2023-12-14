#include "OpenGLPG_Base.h"

#include "TileMeshEditorWidget.h"

#include "Assert.h"
#include "Dodec.h"
#include "ImGuiWidgets.h"

#include <tuple>

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

    DrawEditing();

    changed |= DrawReset();

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

void TileMeshEditorWidget::DrawEditing()
{
    if (mySelectedVertex != -1)
    {
        const TileVertex::Data* vertexData {myTileMesh->GetVertexData(mySelectedVertex)};
        const Dodec& coords {myTileMesh->GetCoordinates(mySelectedVertex)};
        const auto [o, i, n, in] {coords.GetCoords()};
        const Vec2 pos {coords.GetPos()};
        ASSERT(vertexData != nullptr, "Invalid vertex");

        ImGui::Separator();
        ImGui::Text("Selected Vertex");
        ImGui::BeginChild("SelectedVertex", ImVec2(0, 100), true, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Dodec: %d%s + %d%s + %d%s + %d%s", o, "", i, "i", n, "n", in, "in");
        ImGui::Text("Position: %9.5f %9.5f", pos.x, pos.y);
        ImGui::Text("Color: %s", vertexData->myColor ? "B" : "W");
        ImGui::EndChild();
        ImGui::Separator();
    }
    if (ImGui::TreeNode("Editing"))
    {
        ImGui::RadioButton("Info", &myClickAction, 0);
        ImGui::RadioButton("Vertex Color", &myClickAction, 1);
        ImGui::Separator();
        ImGui::TreePop();
    }
}

bool TileMeshEditorWidget::DrawReset()
{
    bool isResetting {false};
    if (ImGui::TreeNode("Reset"))
    {
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
            mySelectedVertex = -1;
        }
        ImGui::Separator();
        ImGui::TreePop();
    }
    return isResetting;
}
