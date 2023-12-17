#include "OpenGLPG_Base.h"

#include "TileMeshEditorWidget.h"

#include "Assert.h"
#include "ImGuiWidgets.h"

#include <string>
#include <tuple>

TileMeshEditorWidget::TileMeshEditorWidget()
{
    myNormComparisonData[0] = 0;
    myNormComparisonData[1] = 0;
}

void TileMeshEditorWidget::AttachTileMeshObject(TileMesh& aTileMesh)
{
    myTileMesh = &aTileMesh;
    myHeightToDisplay = myTileMesh->GetMaxHeight();
}

void TileMeshEditorWidget::Draw()
{
    ASSERT(myTileMesh != nullptr, "Failed attaching tilemesh object");
    bool changed {false};

    changed |= DrawShowdata();

    DrawEditing();

    changed |= DrawReset();

    if (ImGui::Button("Inflate"))
    {
        myTileMesh->SubdivideAllFaces();
        ++myHeightToDisplay;
        changed = true;
    }
    changed |= ImGui::SliderInt("##Display Height", &myHeightToDisplay, 0, myTileMesh->GetMaxHeight(),
                                "Display Height = %d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::Separator();

    changed |= DrawBrushes();

    if (changed)
    {
        myOnChanged();
    }
}

bool TileMeshEditorWidget::DrawShowdata()
{
    bool changed {false};

    ImGui::Text("Show");
    changed |= ImGui::Checkbox("Graphs", &myShowGraphs);
    changed |= ImGui::Checkbox("Edges", &myShowEdges);
    changed |= Widgets::RadioButton("None", &myShowBlocks, ShowBlocks::None);
    ImGui::SameLine();
    changed |= Widgets::RadioButton("Vertices", &myShowBlocks, ShowBlocks::Vertices);
    ImGui::SameLine();
    changed |= Widgets::RadioButton("Faces", &myShowBlocks, ShowBlocks::Faces);

    return changed;
}

void TileMeshEditorWidget::DrawEditing()
{
    if (mySelectedVertex != -1 && myActionMode == ActionMode::Vertices)
    {
        const TileVertex::Data* vertexData {myTileMesh->GetVertexData(mySelectedVertex)};
        const Dodec& coords {myTileMesh->GetCoordinates(mySelectedVertex)};
        ASSERT(vertexData != nullptr, "Invalid vertex");

        ImGui::Separator();
        ImGui::Text("Selected Vertex");
        ImGui::BeginChild("SelectedVertex", ImVec2(0, 120), true, ImGuiWindowFlags_AlwaysAutoResize);
        DrawCoordinates(coords);
        ImGui::Text("Color: %s", vertexData->myColor ? "B" : "W");
        ImGui::EndChild();
        ImGui::Separator();
    }
    else if (mySelectedFace != -1 && myActionMode == ActionMode::Faces)
    {
        const TileFace::Data* faceData {myTileMesh->GetFaceData(mySelectedFace)};
        ASSERT(faceData != nullptr, "Invalid face");

        ImGui::Separator();
        ImGui::Text("Selected Face");
        ImGui::BeginChild("SelectedFace", ImVec2(0, 30), true, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Color: %s", faceData->myColor ? "B" : "W");
        ImGui::EndChild();
        ImGui::Separator();
    }

    if (ImGui::TreeNode("Editing"))
    {
        ImGui::Text("Mode");
        Widgets::RadioButton("Vertices", &myActionMode, ActionMode::Vertices);
        ImGui::SameLine();
        Widgets::RadioButton("Faces", &myActionMode, ActionMode::Faces);

        ImGui::Separator();
        ImGui::Text("Action");
        Widgets::RadioButton("Inspect", &myClickAction, ClickAction::Inspect);
        Widgets::RadioButton("Color", &myClickAction, ClickAction::FlipColor);
        Widgets::RadioButton("Black", &myClickAction, ClickAction::PaintBlack);
        Widgets::RadioButton("White", &myClickAction, ClickAction::PaintWhite);
        ImGui::Separator();
        ImGui::TreePop();
    }
}

bool TileMeshEditorWidget::DrawBrushes()
{
    bool changed {false};

    ImGui::Separator();
    ImGui::PushID("BrushRandom");
    changed |= DrawBrushRandom();
    ImGui::PopID();

    ImGui::Separator();
    ImGui::PushID("BrushNorm");
    changed |= DrawBrushNorm();
    ImGui::PopID();

    ImGui::Separator();
    return changed;
}

bool TileMeshEditorWidget::DrawBrushRandom()
{
    ImGui::SliderFloat("##Random Color", &myVertexColorThreshold, 0., 1., "Randomize Vertices | p = %.3f",
                       ImGuiSliderFlags_AlwaysClamp);
    ImGui::SameLine();
    if (ImGui::Button("Run"))
    {
        myTileMesh->RandomizeVertexColors(myVertexColorThreshold);
        return true;
    }
    return false;
}

bool TileMeshEditorWidget::DrawBrushNorm()
{
    std::string comparison = "";
    switch (myNormSelectionType)
    {
    case ComparisonType::Less:
        comparison = "<";
        break;
    case ComparisonType::Equal:
        comparison = "=";
        break;
    case ComparisonType::Greater:
        comparison = ">";
        break;
    default:
        break;
    }
    int normComparison {myNormComparisonData[0] * myNormComparisonData[0] +
                        myNormComparisonData[1] * myNormComparisonData[1]};
    ImGui::Text("Norm %s %d", comparison.c_str(), normComparison);

    bool edited {false};
    ImGui::SameLine();
    if (ImGui::Button("<"))
    {
        myNormSelectionType = ComparisonType::Less;
        edited = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("="))
    {
        myNormSelectionType = ComparisonType::Equal;
        edited = true;
    }
    ImGui::SameLine();
    if (ImGui::Button(">"))
    {
        myNormSelectionType = ComparisonType::Greater;
        edited = true;
    }
    edited |= ImGui::SliderInt2("##Paint Norm1", &myNormComparisonData[0], 0, 300);
    normComparison =
        myNormComparisonData[0] * myNormComparisonData[0] + myNormComparisonData[1] * myNormComparisonData[1];

    if (edited)
    {
        switch (myNormSelectionType)
        {
        case ComparisonType::Less: {
            myTileMesh->ColorVerticesSatisfying(
                [normComparison](const TileVertex& aVertex) { return aVertex.myCoordinates.Norm() < normComparison; });
            break;
        }
        case ComparisonType::Equal: {
            myTileMesh->ColorVerticesSatisfying(
                [normComparison](const TileVertex& aVertex) { return aVertex.myCoordinates.Norm() == normComparison; });
            break;
        }
        case ComparisonType::Greater: {
            myTileMesh->ColorVerticesSatisfying(
                [normComparison](const TileVertex& aVertex) { return aVertex.myCoordinates.Norm() > normComparison; });
            break;
        }
        default:
            break;
        }
        return true;
    }
    return false;
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
    const Dodec conj {aDodec.Conj()};
    const int norm {aDodec.Norm()};

    Widgets::RadioButton("R[i]", &myDodecDisplayStyle, DodecDisplayStyle::RealI);
    ImGui::SameLine();
    Widgets::RadioButton("Z[i,n]", &myDodecDisplayStyle, DodecDisplayStyle::IntegerIN);
    ImGui::SameLine();
    Widgets::RadioButton("Z[i,z]", &myDodecDisplayStyle, DodecDisplayStyle::IntegerIZ);

    DrawDodec("Dodec", aDodec);
    ImGui::Text("Norm: %d", norm);
    DrawDodec("Norm2", aDodec * aDodec.Conj());
}

void TileMeshEditorWidget::DrawDodec(const char* aName, const Dodec& aDodec)
{
    switch (myDodecDisplayStyle)
    {
    case DodecDisplayStyle::RealI: {
        const Vec2 pos {aDodec.Pos()};
        ImGui::Text("%s: %9.5f%s + %9.5f%s", aName, pos.x, "", pos.y, "i");
        break;
    }
    case DodecDisplayStyle::IntegerIN: {
        const auto [x, y, z, w] {aDodec.GetCoordsININ()};
        ImGui::Text("%s: %d%s + %d%s + %d%s + %d%s", aName, x, "", y, "i", z, "n", w, "in");
        break;
    }
    case DodecDisplayStyle::IntegerIZ: {
        const auto [x, y, z, w] {aDodec.GetCoordsIZIZ()};
        ImGui::Text("%s: %d%s + %d%s + %d%s + %d%s", aName, x, "", y, "i", z, "z", w, "iz");
        break;
    }
    }
}
