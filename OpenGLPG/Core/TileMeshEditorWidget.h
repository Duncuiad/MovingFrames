#pragma once

#include "Dodec.h"
#include "EditorWidget.h"
#include "ThresholdWidgetBlock.h"
#include "TileMesh.h"

#include <memory>
#include <vector>

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
    void CreateThresholdBlocks();

    bool DrawShowdata();
    void DrawEditing();
    bool DrawBrushes();
    bool DrawBrushRandom();
    bool DrawReset();
    void DrawCoordinates(const Dodec& aDodec);
    void DrawDodec(const char* aName, const Dodec& aDodec);

    enum class DodecDisplayStyle {
        RealI,
        IntegerIN,
        IntegerIZ
    };

    std::vector<std::unique_ptr<ThresholdWidgetBlock<TileVertex>>> myThresholdBlocks;
    DodecDisplayStyle myDodecDisplayStyle {DodecDisplayStyle::IntegerIN};
    float myVertexColorThreshold {0.f};
};
