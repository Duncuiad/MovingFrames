#pragma once

#include "Dodec.h"
#include "EditorWidget.h"
#include "TileMesh.h"

class TileMeshEditorWidget : public EditorWidget
{
public:
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
        FlipColor,
        PaintBlack,
        PaintWhite
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
    bool DrawReset();
    void DrawCoordinates(const Dodec& aDodec);

    enum class DodecDisplayStyle {
        RealI,
        IntegerIN,
        IntegerIZ
    };

    DodecDisplayStyle myDodecDisplayStyle {DodecDisplayStyle::IntegerIN};
    float myVertexColorThreshold {0.f};
};
