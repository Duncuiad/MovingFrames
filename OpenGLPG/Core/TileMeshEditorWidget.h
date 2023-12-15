#pragma once

#include "Dodec.h"
#include "EditorWidget.h"
#include "TileMesh.h"

class TileMeshEditorWidget : public EditorWidget
{
public:
    void AttachTileMeshObject(TileMesh& aTileMesh);
    void Draw() override;

    TileMesh* myTileMesh {nullptr};
    int myHeightToDisplay {0};
    int myClickAction {0};
    int myDodecDisplayStyle {1};
    int mySelectedVertex {-1};
    bool myShowGraphs {false};
    bool myShowEdges {true};
    bool myShowBlocks {true};
    float myVertexColorThreshold {0.f};

private:
    void DrawEditing();
    bool DrawReset();
    void DrawCoordinates(const Dodec& aDodec);
};
