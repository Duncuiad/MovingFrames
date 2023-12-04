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

    ImGui::Text("Reset");
    if (ImGui::Button("Triangle A"))
    {
        myTileMesh->Reset(TileType::TriangleA);
        changed = true;
    }
    if (ImGui::Button("Triangle B"))
    {
        myTileMesh->Reset(TileType::TriangleB);
        changed = true;
    }
    if (ImGui::Button("Square A"))
    {
        myTileMesh->Reset(TileType::SquareA);
        changed = true;
    }
    if (ImGui::Button("Square B"))
    {
        myTileMesh->Reset(TileType::SquareB);
        changed = true;
    }
    if (ImGui::Button("Square C"))
    {
        myTileMesh->Reset(TileType::SquareC);
        changed = true;
    }
    ImGui::Separator();

    if (ImGui::Button("Subdivide"))
    {
        myTileMesh->SubdivideAllFaces();
    }

    if (changed)
    {
        myOnChanged();
    }
}
