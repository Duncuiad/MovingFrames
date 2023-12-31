#pragma once

#include "Dodec.h"
#include "EditorWidget.h"
#include "TileMesh.h"

class TileMeshEditorWidget : public EditorWidget
{
public:
    TileMeshEditorWidget();
    void AttachTileMeshObject(TileMesh& aTileMesh);
    void Draw() override;

    enum class ShowBlocks {
        None,
        Vertices,
        Faces
    };

    enum class ActionMode {
        Vertices,
        Faces
    };

    enum class ClickAction {
        Inspect,
        Paint,
    };

    TileMesh* myTileMesh {nullptr};

    int myHeightToDisplay {0};
    bool myShowGraphs {false};
    bool myShowEdges {true};
    ShowBlocks myShowBlocks {ShowBlocks::Vertices};
    ActionMode myActionMode {ActionMode::Vertices};
    ClickAction myClickAction {ClickAction::Inspect};
    int mySelectedVertex {-1};
    int mySelectedFace {-1};

private:
    bool DrawShowdata();
    void DrawEditing();
    bool DrawBrushes();
    bool DrawBrushRandom();
    bool DrawBrushNorm();
    bool DrawBrushNormAlt();
    bool DrawBrushHeight();
    bool DrawReset();
    void DrawCoordinates(const Dodec& aDodec);
    void DrawDodec(const char* aName, const Dodec& aDodec);

    enum class DodecDisplayStyle {
        RealI,
        IntegerIN,
        IntegerIZ
    };

    enum class ComparisonType {
        Less,
        Equal,
        Greater
    };

    DodecDisplayStyle myDodecDisplayStyle {DodecDisplayStyle::IntegerIN};
    ComparisonType myNormSelectionType {ComparisonType::Equal};
    ComparisonType myNormAltSelectionType {ComparisonType::Equal};
    ComparisonType myHeightSelectionType {ComparisonType::Equal};
    int myNormComparisonData[2];
    float myNormAltComparisonData {0};
    int myHeightComparisonData {0};
    float myVertexColorThreshold {0.f};
};
