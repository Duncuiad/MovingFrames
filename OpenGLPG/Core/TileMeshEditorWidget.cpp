#include "OpenGLPG_Base.h"

#include "TileMeshEditorWidget.h"

#include "Assert.h"
#include "ImGuiWidgets.h"
#include "SliderThresholdWidgetBlock.h"

#include <array>
#include <string>
#include <tuple>

TileMeshEditorWidget::TileMeshEditorWidget()
{
    CreateThresholdBlocks();
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

void TileMeshEditorWidget::CreateThresholdBlocks()
{
    myThresholdBlocks.emplace_back(std::make_unique<Int2SliderThresholdWidgetBlock<TileVertex>>(
        "Norm", [](const TileVertex& aVertex) { return static_cast<float>(aVertex.myCoordinates.Norm()); },
        [this](const auto& aPredicate) { myTileMesh->ColorVerticesSatisfying(aPredicate); },
        []() {
            return std::array<int, 2> {0, 0};
        },
        []() {
            return std::array<int, 2> {300, 300};
        }));

    myThresholdBlocks.emplace_back(std::make_unique<FloatSliderThresholdWidgetBlock<TileVertex>>(
        "Alt Norm",
        [](const TileVertex& aVertex) {
            return 0.5f * log2((aVertex.myCoordinates.ConjNM() * aVertex.myCoordinates.ConjNM().Conj()).Pos().x);
        },
        [this](const auto& aPredicate) { myTileMesh->ColorVerticesSatisfying(aPredicate); }, []() { return 0.f; },
        []() { return 10.f; }));

    myThresholdBlocks.emplace_back(std::make_unique<IntSliderThresholdWidgetBlock<TileVertex>>(
        "Height", [](const TileVertex& aVertex) { return static_cast<float>(aVertex.myHeight); },
        [this](const auto& aPredicate) { myTileMesh->ColorVerticesSatisfying(aPredicate); }, []() { return -1; },
        [this]() { return myTileMesh->GetMaxHeight() + 1; }));
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
        ImGui::BeginChild("SelectedVertex", ImVec2(0, 150), true, ImGuiWindowFlags_AlwaysAutoResize);
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
        Widgets::RadioButton("Paint", &myClickAction, ClickAction::Paint);
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

    for (auto& block : myThresholdBlocks)
    {
        ImGui::Separator();
        changed |= block->Draw();
    }

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

    Widgets::RadioButton("R[i]", &myDodecDisplayStyle, DodecDisplayStyle::RealI);
    ImGui::SameLine();
    Widgets::RadioButton("Z[i,n]", &myDodecDisplayStyle, DodecDisplayStyle::IntegerIN);
    ImGui::SameLine();
    Widgets::RadioButton("Z[i,z]", &myDodecDisplayStyle, DodecDisplayStyle::IntegerIZ);

    DrawDodec("Dodec", aDodec);
    ImGui::Text("Norm: %d", aDodec.Norm());
    ImGui::Text("Log2(Sqrt(AltNorm)): %.3f", 0.5f * log2((aDodec.ConjNM() * aDodec.ConjNM().Conj()).Pos().x));
    DrawDodec("Norm2", aDodec * aDodec.Conj());
    DrawDodec("NormNM", aDodec * aDodec.ConjNM());
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
