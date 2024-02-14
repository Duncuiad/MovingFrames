#pragma once

#include "Dodec.h"
#include "EditorWidget.h"
#include "ThresholdWidgetBlock.h"
#include "TileMesh.h"

#include <memory>
#include <optional>
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

    enum class ClickAction {
        Inspect,
        Paint,
        Neighbours
    };

    TileMesh* myTileMesh {nullptr};

    int myHeightToDisplay {0};
    bool myShowGraphs {false};
    bool myShowEdges {true};
    bool myCanDeflate {true};
    ShowBlocks myShowBlocks {ShowBlocks::Vertices};
    ClickAction myClickAction {ClickAction::Inspect};
    Vec4 myPaintPrimaryColor {0.f, 0.f, 0.f, 1.f};
    Vec4 myPaintSecondaryColor {1.f, 1.f, 1.f, 1.f};
    std::optional<Dodec> mySelectedVertex;
    int mySelectedFace {-1};

private:
    void CreateThresholdBlocks();

    bool DrawShowdata();
    void DrawEditing();
    void DrawPaintSettings();
    bool DrawBrushes();
    bool DrawBrushRandom();
    bool DrawVertexSpecificBrushes();
    bool DrawInvestigationBrushes();
    bool DrawReset();
    void DrawCoordinates(const Dodec& aDodec);
    void DrawDodec(const char* aName, const Dodec& aDodec);

    enum class DodecDisplayStyle {
        RealI,
        IntegerIN,
        IntegerIZ,
        PowersP
    };

    std::vector<std::unique_ptr<ThresholdWidgetBlock<TileVertex::Pair>>> myThresholdBlocks;
    DodecDisplayStyle myDodecDisplayStyle {DodecDisplayStyle::IntegerIN};
    float myRandomColorThreshold {0.f};
};
