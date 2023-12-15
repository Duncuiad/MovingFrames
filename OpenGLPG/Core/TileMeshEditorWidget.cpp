#include "OpenGLPG_Base.h"

#include "TileMeshEditorWidget.h"

#include "Assert.h"
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
        ASSERT(vertexData != nullptr, "Invalid vertex");

        ImGui::Separator();
        ImGui::Text("Selected Vertex");
        ImGui::BeginChild("SelectedVertex", ImVec2(0, 90), true, ImGuiWindowFlags_AlwaysAutoResize);
        DrawCoordinates(coords);
        ImGui::Text("Color: %s", vertexData->myColor ? "B" : "W");
        ImGui::EndChild();
        ImGui::Separator();
    }
    if (ImGui::TreeNode("Editing"))
    {
        Widgets::RadioButton("Inspect", &myClickAction, ClickAction::Inspect);
        Widgets::RadioButton("Vertex Color", &myClickAction, ClickAction::VertexColor);
        Widgets::RadioButton("Vertex Black", &myClickAction, ClickAction::VertexBlack);
        Widgets::RadioButton("Vertex White", &myClickAction, ClickAction::VertexWhite);
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

void TileMeshEditorWidget::DrawCoordinates(const Dodec& aDodec)
{
    const Dodec norm2 {aDodec * aDodec.Conj()};

    Widgets::RadioButton("R[i]", &myDodecDisplayStyle, DodecDisplayStyle::RealI);
    ImGui::SameLine();
    Widgets::RadioButton("Z[i,n]", &myDodecDisplayStyle, DodecDisplayStyle::IntegerIN);
    ImGui::SameLine();
    Widgets::RadioButton("Z[i,z]", &myDodecDisplayStyle, DodecDisplayStyle::IntegerIZ);

    switch (myDodecDisplayStyle)
    {
    case DodecDisplayStyle::RealI: {
        const Vec2 pos {aDodec.Pos()};
        const Vec2 npos {norm2.Pos()};
        ImGui::Text("Dodec: %9.5f%s + %9.5f%s", pos.x, "", pos.y, "i");
        ImGui::Text("Norm2: %9.5f%s + %9.5f%s", npos.x, "", npos.y, "i");
        break;
    }
    case DodecDisplayStyle::IntegerIN: {
        const auto [x, y, z, w] {aDodec.GetCoordsININ()};
        const auto [nx, ny, nz, nw] {norm2.GetCoordsININ()};
        ImGui::Text("Dodec: %d%s + %d%s + %d%s + %d%s", x, "", y, "i", z, "n", w, "in");
        ImGui::Text("Norm2: %d%s + %d%s + %d%s + %d%s", nx, "", ny, "i", nz, "n", nw, "in");
        break;
    }
    case DodecDisplayStyle::IntegerIZ: {
        const auto [x, y, z, w] {aDodec.GetCoordsIZIZ()};
        const auto [nx, ny, nz, nw] {norm2.GetCoordsIZIZ()};
        ImGui::Text("Dodec: %d%s + %d%s + %d%s + %d%s", x, "", y, "i", z, "z", w, "iz");
        ImGui::Text("Norm2: %d%s + %d%s + %d%s + %d%s", nx, "", ny, "i", nz, "z", nw, "iz");
        break;
    }
    }
}
