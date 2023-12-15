#pragma once

#include "Dodec.h"
#include "EditorWidget.h"
#include "TileMesh.h"

class TileMeshEditorWidget : public EditorWidget
{
public:
    void AttachTileMeshObject(TileMesh& aTileMesh);
    void Draw() override;

    enum class ClickAction {
        Inspect,
        VertexColor,
        VertexBlack,
        VertexWhite
    };

    TileMesh* myTileMesh {nullptr};
    int mySelectedVertex {-1};

    int myHeightToDisplay {0};
    bool myShowGraphs {false};
    bool myShowEdges {true};
    bool myShowBlocks {true};
    ClickAction myClickAction {ClickAction::Inspect};

private:
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
